#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

///////   /////////////  \\\\\\/
///////   G L O B A L S  \\\\\\/
///////   /////////////  \\\\\\/

int isExit = 0;


struct msgbuff
{
    long mtype;
    char mtext[256];
};

void exitHandler(int signum){
    
    isExit = 1;
}


int main()
{
    // Client code

    pid_t Clientpid = getpid();
    key_t msgqidUP;
    key_t upQueue = 100;

    
    signal(SIGINT,exitHandler);

    while(isExit == 0){


        // UP QUEUE //  --- SEND
            msgqidUP = msgget(upQueue, IPC_CREAT | 0644); // or msgget(12613, IPC_CREAT | 0644)
        if(msgqidUP == -1)
        {
            perror("Error in create");
            exit(-1);
        }
        printf("msgqid = %d\n", msgqidUP);

        char str[256];
        printf("Enter your msg : ");
        scanf("%s",str);

        struct msgbuff message;
        message.mtype = 7;     	/* arbitrary value */
        strcpy(message.mtext, str);

        int send_val;
        message.mtype = Clientpid;
        send_val = msgsnd(msgqidUP, &message, sizeof(message.mtext), !IPC_NOWAIT);

        if(send_val == -1){
            perror("Errror in send");
        }


        // DOWN QUEUE // --- RCV
        key_t msgqidDOWN;
        key_t downQueue = 200;

        msgqidDOWN = msgget(downQueue, IPC_CREAT | 0644); // or msgget(12613, IPC_CREAT | 0644)
        if(msgqidDOWN == -1)
        {
            perror("Error in create");
            exit(-1);
        }
        printf("msgqid = %d\n", msgqidDOWN);

        int rec_val;

        /* receive all types of messages */
        rec_val = msgrcv(msgqidDOWN, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);

        if(rec_val == -1)
            perror("Error in receive");
        
        else{
            if (message.mtype == Clientpid){
            printf("\nMessage received: %s\n", message.mtext);
            }
        }
    }

    /*// Destroy msg queue
        printf("Deleting message queue.\n");
        int rem_msgUP = msgctl(msgqidUP, IPC_RMID, (struct msqid_ds *) 0);
        int rem_msgDOWN = msgctl(msgqidDOWN, IPC_RMID, (struct msqid_ds *) 0);

        // Making sure of successful deletion
        if (rem_msgUP == -1) {
            perror("Error in deleting message queue UP.\n");
        }
        else {
            printf("Message queue deleted successfully.\n");
        }

        if (rem_msgDOWN == -1) {
            perror("Error in deleting message queue DOWN.\n");
        }
        else {
            printf("Message queue deleted successfully.\n");
        }*/

    

    return 0;
}
