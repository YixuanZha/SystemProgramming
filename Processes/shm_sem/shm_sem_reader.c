// 设置一个两个信号量来实现 生产者-消费者 模型
// 设置一个空的信号量 -> sem_empty -------> 共享内存是否能被写入
//// sem_empty 初始值为 1 (一开始缓存区为空，可以写)， 生产者(writer) 写入数据之前必须获得这个许可(sem_wait), 消费者(reader) 读完数据之后释放这个许可(sem_post)
// 设置一个满的信号量 -> sem_full --------> 共享内存是否有数据可读
//// sem_full 初始值为 0(一开始缓存区为空，没有数据可读)， 消费者在读取这个数据之前必须等待通知(sem_wait), 生产者在写入数据之后，会发出通知(sem_post) --> 写好，可读

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
    sem_t *empty_sem, *full_sem; // full_sem = 1; sempty_sem = 0
    char local_buffer[BUFFER_SIZE];

    key = ftok(SHM_KEY_PATH, SHM_KEY_ID);
    if (key == -1)
    {
        perror("ftok() failed");
        return 1;
    }

    shmid = shmget(key, BUFFER_SIZE, 0666);
    if (shmid == -1)
    {
        perror("shmget() failed");
        return 1;
    }

    shm_ptr = (shared_memory *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1)
    {
        perror("shmat() failed");
        return 1;
    }

    empty_sem = sem_open(SEM_EMPTY_NAME, 0);
    full_sem = sem_open(SEM_FULL_NAME, 0);

    printf("Receiver: Connected to the shared memory, waiting for message\n");

    while (1)
    {
        sem_wait(full_sem); // full_sem = 0, count = 0 -> the process blocked -> wait for wariter to post(full_sem) 

        if (strncmp(shm_ptr->buffer, "quit", 4) == 0)
        {
            printf("Receiver: Received the quit command, quit!!\n");
            break;
        }
        strncpy(local_buffer, shm_ptr->buffer, shm_ptr->data_size);
        local_buffer[shm_ptr->data_size] = '\0';
        printf("Receiver: The data is ready!! The message is '%s' \n\n", local_buffer);

        sem_post(empty_sem); // empty_sem = 0 -> post(), empty_sem ++ ->writer can write to shared memory

        printf("Receiver: Resetted data ready, waiting for new messages\n");
    }

    sem_close(empty_sem);
    sem_close(full_sem);
    if (shmdt(shm_ptr) == -1)
    {
        perror("shmdt() failed");
        exit(1);
    }

    sem_unlink(SEM_EMPTY_NAME);
    sem_unlink(SEM_FULL_NAME);
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl() failed");
        exit(1);
    }
    printf("Receiver: Destroyed the shared memory,exit....\n");
    return 0;
}