#ifndef MSG_COMMON_
#define MSG_COMMON_

#include <sys/types.h>

#define MSG_KEY_PATH "/tmp/mag_queue_key" 

#define MSG_KEY_ID 'A' // key值是唯一标识符，相当于门牌号

#define MAX_MSG_SIZE 256

struct msg_buffer
{
    long msg_type;                  // 消息类型 
    char msg_text[MAX_MSG_SIZE];    // 消息
};

#endif