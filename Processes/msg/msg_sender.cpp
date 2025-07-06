// key_t ftok(const char *__pathname, int __proj_id)
// __pathname -> 一个已存在的文件路径 || __proj_id -> 一个项目id ('A')
// 返回一个key值

// int msgget(key_t __key, int __msgflg)
// key -> 通过ftok()创建的key值 || msgflg -> 标志位 0666 | IPC_CREAT -> 若队列不存在则创造一个队列

// Sender: 通过msgsnd() 来把消息放入队列
// Receiver: 通过msgrcv() 从队列中取出消息 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_common.h"

int main()
{
    key_t key;
    int msgid;
    struct msg_buffer message;
    key = ftok(MSG_KEY_PATH,MSG_KEY_ID); // 生成唯一的key
    if(key == -1)
    {
        perror("ftok() failed");
        return 1;
    }

    msgid = msgget(key,0666); // 把key值交给内核，换取一个消息队列标识符 ---> msgid(消息队列id)
    if(msgid == -1)
    {
        perror("msgget() failed failed -- Please make sure that the receiver program is ruuning!!");
        return 1;
    }

    printf("Sender: I have connected to the message queue\n\n");
    printf("Pelease enter the word you want to send\n");

    while (1)
    {
        printf("> ");
        if(fgets(message.msg_text,MAX_MSG_SIZE,stdin) == NULL)  // 从文件流中读一行文本， 并安全地存在字符数组中
        {
            break;
        }
        message.msg_type = 1; // 消息分类的标签

        if(strncmp(message.msg_text ,"quit",4) == 0) // 先判断是否发送的字符串为 quit
        {
            break;
        }

        if(msgsnd(msgid,&message,strlen(message.msg_text) + 1,0) == -1) // 把message 结构体的内容发送到 msgid 的队列中
        {
            break;
        }
    }
    
    printf("Sender: Exit.......\n");
    return 0;
}