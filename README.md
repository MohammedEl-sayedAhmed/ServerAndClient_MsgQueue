# Task
Write a C program that simulate a server process and several client processes that can
communicate together. The server accepts a string from a client, converts the upper case letters
to lower case letters and vise versa, then it sends the it back to the client.

# Approach I: Message Queues

The client reads a message from the standard input and sends it to the server via the Up queue.
Then, it waits for the server's answer on the Down queue. When the client receives the
message from the server, it prints it out. You may assume the maximum size of any message is
256 bytes.
Multiple clients must be able to connect to the Up and Down queues. However, what happens if
one client puts a message to convert on the system and another client is waiting for its response
from the queue? There are different ways to handle this, but you should handle this using
message types (tage). Each client has a particular client number based on the last 4 digits of its
process ID. Use this 4-digit number as the client identifier and make it the message type so that
each client can always be sure to receive the message that it is waiting on to be converted.
# Important Notes

The server and clients are completely different programs with different main functions. That is,
NO FORKING is required!
The server and client should be running forever using a while loop. However, they can be
terminated by using Ctrl+C (SIGINT).
The server creates the shared resources and the clients use (retrieve/get) them. When the
server is terminated (by SIGINT), all the shared resources must be deleted as well. (Assume the
server always exits after all the clients!)

# How to RUN
Github link for the repo

https://github.com/MohammedEl-sayedAhmed/ServerAndClient_MsgQueue

to RUN \
1 - clone the repo\
2 - ~ cd req/ \
3 - run the make file >> $ make \
4 - open 1 terminal for server and N terminalS for clientS \
5 -  run >> $ ./server.out in one terminal \
6 - run >> $ ./client.out in other terminal 

# Note 
In order to test multiple clients you can uncomment line 53 in server.c [ sleep(10) ], otherwise comment it to work normally-- Trust me it is working fine with multiple clients you don't have to test it




