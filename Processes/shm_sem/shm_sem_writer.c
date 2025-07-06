// empty_sem = 1 -> the writer can write to the shared memory at first -> reader(wait(empty_sem)) -> reader can read from the shared memory 
// full_sem = 0 -> reader can not read from the shared memory at first

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "shm_sem_common.h"

int main()
{
    key_t key;
    int shmid;
    shared_memory *shm_ptr;
    sem_t *empty_sem, *full_sem;

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

    empty_sem = sem_open(SEM_EMPTY_NAME,O_CREAT,0666,1);
    full_sem = sem_open(SEM_FULL_NAME,O_CREAT,0666,0);

    printf("Sender: Connect to the shared memory success\n");
    printf("please enter the word\n");

    while (1)
    {
        printf("> ");
        if(fgets(shm_ptr->buffer,BUFFER_SIZE,stdin) == 0)
        {
            perror("fgets() failed");
            break;
        }
        
        shm_ptr->data_size = strlen(shm_ptr->buffer);

        sem_wait(empty_sem); // empty_sem = 1 -> empty_sem -- -> the writer can write to shared memory 

        printf("Sender: write to the shared memory success\n");

        sem_post(full_sem); // full_sem ++ -> tell the reader can read from shared memory

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
    sem_close(empty_sem);
    sem_close(full_sem);

    printf("Sender: 已分离共享内存，程序退出\n");
    return 0;
}