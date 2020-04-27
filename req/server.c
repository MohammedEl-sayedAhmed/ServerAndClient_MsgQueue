#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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

/* convert upper case to lower case or vise versa */
void conv(char *msg)
{
    int size = strlen(msg);
    for (int i = 0; i < size; i++)
        if (islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] = tolower(msg[i]);
}

void exitHandler(int signum){
    
    isExit = 1;
}


int main()
{
    // Server code

    pid_t Serverpid = getpid();
    key_t msgqidUP;
    key_t msgqidDOWN;
    
    
    signal(SIGINT,exitHandler);

    while (isExit == 0){

        //sleep(10);
        // UP QUEUE // 
        key_t upQueue = 100;

        msgqidUP = msgget(upQueue, IPC_CREAT | 0644); // or msgget(12613, IPC_CREAT | 0644)
        if(msgqidUP == -1)
        {
            perror("Error in create");
            exit(-1);
        }
        printf("msgqid = %d\n", msgqidUP);

        int rec_val;
        struct msgbuff message;

        /* receive all types of messages */
        rec_val = msgrcv(msgqidUP, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);

        if(rec_val == -1)
            perror("Error in receive");
        else
            {
                printf("\nMessage received: %s\n", message.mtext);
            }
        
        char toBeConv[256];
        strcpy(toBeConv, message.mtext);
        conv(toBeConv);
        printf("\nMessage after CONV: %s\n",toBeConv);


        // DOWN QUEUE // 
        
        key_t downQueue = 200;

        msgqidDOWN = msgget(downQueue, IPC_CREAT | 0644); // or msgget(12613, IPC_CREAT | 0644)
        if(msgqidDOWN == -1)
        {
            perror("Error in create");
            exit(-1);
        }
        printf("msgqid = %d\n", msgqidDOWN);
        strcpy(message.mtext,toBeConv);

        int send_val;
        send_val = msgsnd(msgqidDOWN, &message, sizeof(message.mtext), !IPC_NOWAIT);

        if(send_val == -1){
            perror("Errror in send");
        }

    }

    // Destroy msg queue
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
        }
    
    
    

    return 0;
}
