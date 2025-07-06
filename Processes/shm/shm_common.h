#ifndef SHM_COMMON_H_
#define SHM_COMMON_H_

#include <sys/stat.h>

#define SHM_KEY_PATH "/tmp/shm_demo_key"

#define SHM_KEY_ID 'M'

#define BUFFER_SIZE 256

typedef struct
{
    int data_ready; // 0 - unready || 1 - ready
    char buffer[BUFFER_SIZE]; 
} shared_memory;

#endif