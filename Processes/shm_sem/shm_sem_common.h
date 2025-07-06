#ifndef SHM_SEM_COMMON_H_
#define SHM_SEM_COMMON_H_

#include <sys/stat.h>

#define SHM_KEY_PATH "/tmp/shm_demo_key"

#define SHM_KEY_ID 'M'

#define SEM_EMPTY_NAME "/sem_empty"
#define SEM_FULL_NAME "/sem_full"

#define BUFFER_SIZE 256

typedef struct
{
    int data_size;
    char buffer[BUFFER_SIZE]; 
} shared_memory;

#endif