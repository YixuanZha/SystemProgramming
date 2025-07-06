#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include "msg_common.h"

int main()
{
    key_t key;
    int msgid;
    struct msg_buffer message;

    int fd = open(MSG_KEY_PATH,O_CREAT,0666);
    if(fd < 0)
    {
        perror("failed to open the file");
        return 1;
    }

    key = ftok(MSG_KEY_PATH,MSG_KEY_ID);
    if(key == -1)
    {
        perror("ftok() failed");
        return 1;
    }

    msgid = msgget(key,IPC_CREAT | 0666);
    if(msgid == -1)
    {
        perror("msgget failed");
        return 1;
    }

    printf("Receiver: Message queue has been created, the id is: %d\n",msgid);
    printf("Receiver: Waiting for messages\n");

    while (1)
    {
        if(msgrcv(msgid,&message,MAX_MSG_SIZE,1,0) == -1) // 接收标签为1 的消息， 如果为0则为先进先出
        {
            perror("msgrcv() failed");
            return 1;
        }

        if(strcmp(message.msg_text,"quit") == 1)
        {
            break;
        }

        printf("Receiver: I have received the message: %s\n",message.msg_text);

    }

     if(msgctl(msgid,IPC_RMID,NULL) == -1)
     {
        perror("msgctl() failed");
        exit(1);
     }

     printf("Receiver: Exit..........\n");
    
    return 0;
}