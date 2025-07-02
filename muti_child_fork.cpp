#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MAX_CHILD 5
#define SEM_NAME "/my_demo_semaphore"

int main()
{
    sem_unlink(SEM_NAME);
    sem_t *sem = sem_open(SEM_NAME,O_CREAT,0644,0);
    if(sem == SEM_FAILED)
    {
        perror("sem_open() failed");
        exit(1);
    }
    for(int i = 0; i < MAX_CHILD; i++)
    {
        pid_t pid = fork();
        if(pid < 0) 
        {
            perror("fork failed");
            exit(1);
        }
        else if(pid == 0)
        {
            sem_t *child_sem = sem_open(SEM_NAME,0);

            printf("This is child process '%d', my parent process pid is: %d, my pid is: %d\n",i + 1,getppid(),getpid());

            sem_post(child_sem); // sem count +1
            sem_close(child_sem);

            sleep(2);
            exit(0);// exit fromt the child process
        }
    }
    printf("Parent process: all of the child process are created\n");
    sleep(2);
    printf("parent process are receving the signal from child process\n");
    for(int i = 0; i < MAX_CHILD; i++)
    {
        sem_wait(sem); // if sem_count > 0. else if parent process was blocked
        printf("parent process have received %dth siganl from child process\n",i + 1);
    }

   


    for(int i = 0; i < MAX_CHILD; i++)
    {
        int status;
        pid_t terminated_pid = wait(&status);

        if(terminated_pid > 0) 
        {
            if(WIFEXITED(status))
            {
                printf("parent process: I have found that the child process %d has exited\n",terminated_pid);
            }
            else
            {
                printf("parent process: The child process %d has failed to exit\n",terminated_pid);
            }
        }
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);
    return 0;

}