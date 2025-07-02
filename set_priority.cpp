#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>

void cpu_intensive_work()
{
    printf("Child process --- %d is doing a cpu intensive work\n",getpid());
    while(1)
    {

    }
}

int main()
{
    pid_t child_a, child_b;

    printf("Parent process (PID: %d) starting ....\n",getpid());

    child_a = fork();

    if(child_a < 0)
    {
        perror("fork child_a failed");
        exit(1);
    }
    else if(child_a == 0)
    {
        printf("Child process A (PID: %d), priority is %d\n",getpid(), getpriority(PRIO_PROCESS,0));
        cpu_intensive_work();
        exit(0);
    }

    child_b = fork();
    if(child_b < 0)
    {
        perror("fork child_b failed");
        exit(1);
    }
    else if(child_b == 0)
    {
        int ori_nice;
        int set_nice = 15;

        ori_nice = getpriority(PRIO_PROCESS,0);
        printf("Child process B (PID: %d), priority is %d\n",getpid(), ori_nice);

        if(setpriority(PRIO_PROCESS,0,set_nice) == -1)
        {
            perror("setpritority failed");
        }
        else
        {
            printf("Child process B (PID: %d), set priority to %d\n",getpid(), getpriority(PRIO_PROCESS,0));
        }
        cpu_intensive_work();
        exit(0);
    }

    printf("Parent procrss: create child process A and B success\n");
    printf("Open a new terminal, and run 'top -p %d,%d'\n",child_a,child_b);

    waitpid(child_a,NULL,0);
    waitpid(child_b,NULL,0);

    return 0;
}


