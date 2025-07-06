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

    key = ftok(SHM_KEY_PATH,SHM_KEY_ID);
    if(key == -1)
    {
        perror("ftok() failed");
        return 1;
    }

    shmid = shmget(key,BUFFER_SIZE,0666);
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

    printf("Receiver: Connected to the shared memory, waiting for message\n");

    while (1)
    {
        if(shm_ptr -> data_ready)
        {
            printf("Receiver: The data is ready!! The message is '%s' \n\n",shm_ptr->buffer);

            if(strncmp(shm_ptr->buffer,"quit",4) == 0)
            {
                printf("Receiver: Received the quit command, quit!!\n");
                break;
            }
            shm_ptr ->data_ready = 0;
            printf("Receiver: Resetted data ready, waiting for new messages\n");
        }
        usleep(100000);

    }
    
    if(shmdt(shm_ptr) == -1)
    {
        perror("shmdt() failed");
        exit(1);
    }

    if(shmctl(shmid,IPC_RMID,NULL) == -1)
    {
        perror("shmctl() failed");
        exit(1);
    }
    printf("Receiver: Destroyed the shared memory,exit....\n");
    return 0;
}