#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PATH "/tmp/my_fifo"
#define BUFFER_SIZE 256

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        printf("failed to make a fifo");
        // return 1;
    }
    else
    {
        printf("Make the fifo successful\n");
    }

    printf("Sender: Waiting for reader opening the fifo\n");

    // --- 1. 以只写方式打开管道 ---
    // O_RDONLY: 只读模式。
    // 如果没有其他进程以读模式打开这个FIFO，这个open()调用会阻塞。
    fd = open(FIFO_PATH, O_WRONLY);

    if (fd < 0)
    {
        perror("open the fifo failed");
        return 1;
    }
    printf("Reveiver has opened the fifo, sender can send message now!!!\n");

    printf("Please enter the message you want to send!!!\n");

    while (1)
    {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }
        if (strncmp(buffer, "quit", 4) == 0)
        {
            break;
        }
        if (write(fd, buffer, strlen(buffer)) == -1)
        {
            perror("Failed to write to the fifo");
            break;
        }
    }

    close(fd);
    printf("Sender: Close the fifo, exit.....\n");

    return 0;
}