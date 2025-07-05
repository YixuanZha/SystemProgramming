// 管道通信只能用于父子进程或者兄弟进程间的通信
// int pipe_fd[2] -> pipe_fd[0] -> Read :: pipe_fd[1] -> Write 
// pipe(pipe_fd) -> create the pipe
// child: Sender -> close pipe_fd[1](Read) -> write(pipe_fd[0],message) -> close(pipe_fd[0]);
// parent: Receiver -> close(pipe_fd[0]) (Write) -> read(pipe_fd[1] ,buffer,sizeof(buffer) - 1) -> close(pipe_fd[1]) (Read)
// ----pipe_fd[0] ---> 是写管道   ----------pipe_fd[1] -----> 是读管道


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main()
{
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    const char* message = "hello from child process";

    if(pipe(pipe_fd) == -1)
    {
        perror("failed to create the pipe");
        return 1;
    }

    pid = fork();
    if(pid < 0)
    {
        perror("failed to fork()");
        return 1;
    }

    if(pid == 0)
    {
        printf("This is child process, pid is: %d\n",getpid());
        close(pipe_fd[0]); // 关闭读端
        printf("Child process: The read port is closed, write message '%s' to the pipe\n",message);
        write(pipe_fd[1],message,strlen(message));
        close(pipe_fd[1]);
        printf("Child process: Send message to the pipe successfully,exit.......\n");
        exit(0);
    }
    else
    {
        printf("This is parent process, pid is: %d\n",getpid());
        close(pipe_fd[1]); // 关闭写端
        printf("Parent process: The write port is close, wait for the message from child process\n");
        ssize_t bytes_read = read(pipe_fd[0],buffer,sizeof(buffer) -1 );
        if(bytes_read <= 0)
        {
            perror("Parent process: failed to read from the pipe");
            return 1;
        }
        else
        {
            buffer[bytes_read] = '\0';
            printf("Parent process: read message from child process: '%s'\n",buffer);
        }
        close(pipe_fd[0]);

        int status;
        wait(&status);
        printf("Parent process: child process has exited, exit.......\n");
        exit(0);
    }

    return 0;
}