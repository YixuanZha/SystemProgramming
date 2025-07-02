#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Entrance of the main function: The pid is: %d\n",getpid());
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) // child process
    {
        printf("Child process is created, the pid is: %d\n",getpid());
        printf("I will execute ls -l / command\n\n");

        char *argv[] = {"ls","-l","/",NULL};

        execvp(argv[0],argv);

        printf("exiting .....\n"); // Can not execute .. because if execvp success never return!!!!

        perror("execvp failed"); 
        exit(1);
    }
    else // parent process
    {
        printf("This is parent process, the pid I created is: %d\n",pid);
        printf("Wait for the child process\n");

        int status;
        wait(&status);

        if(WIFEXITED(status))
        {
            printf("\nParent process: child process has exited, the exit code is: %d\n",WEXITSTATUS(status));
        }
        else
        {
            printf("Child exit error\n");
        }

        printf("Parent process is exiting...\n");

        return 0;
    }
}