#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_common.h"

int main()
{
    key_t key;
    int shmid;
    shared_memory *shm_ptr;

    int fd = open(SHM_KEY_PATH,O_CREAT, 0666);
    if(fd < 0)
    {
        perror("Failed to open the file");
        return 1;
    }

    key = ftok(SHM_KEY_PATH,SHM_KEY_ID);
    if(key == -1)
    {
        perror("ftok() failed");
        return 1;
    }

    shmid = shmget(key,BUFFER_SIZE, 0666 | IPC_CREAT);
    if(shmid == -1)
    {
        perror("shmget() failed");
        return 1;
    }

    shm_ptr = (shared_memory*) shmat(shmid,NULL,0);
    if(shm_ptr == (void*) -1)
    {
        perror("shmat() failed");
        return 1;
    }

    printf("Sender: Connect to the shared memory success\n");

    shm_ptr ->data_ready = 0;
    printf("please enter the word\n");

    while (1)
    {
        printf("> ");
        if(fgets(shm_ptr->buffer,BUFFER_SIZE,stdin) == 0)
        {
            perror("fgets() failed");
            break;
        }
        
        // set the data ready to 1
        shm_ptr->data_ready = 1;
        printf("Sender: write to the shared memory success\n");

        if(strncmp(shm_ptr->buffer,"quit",4) == 0)
        {
            break;
        }
    }
    if(shmdt(shm_ptr) == -1)
    {
        perror("shmdt() failed");
        exit(1);
    }

    printf("Sender: 已分离共享内存，程序退出\n");
    return 0;
}