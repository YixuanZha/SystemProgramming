#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define PATH_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 256

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("Receiver: Trying to open the fifo '%s'\n", PATH_NAME);

    // --- 1. 以只读方式打开管道 ---
    // O_RDONLY: 只读模式。
    // 如果没有其他进程以写模式打开这个FIFO，这个open()调用会阻塞。
    fd = open(PATH_NAME, O_RDONLY);
    if (fd < 0)
    {
        perror("Failed to open the fifo");
        return 1;
    }
    else
    {
        printf("Receiver: Open the fifo successfully, waiting for the message\n");
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("Receiver: Receive the message: %s\n", buffer);
    }

    if (bytes_read == 0)
    {
        printf("Receiver: Sender has close the connection, exit....\n");
    }
    else
    {
        printf("Recevier: failed to read from the fifo\n");
    }

    close(fd);
    unlink(PATH_NAME);
    printf("Receiver: Closed and removed the FIFO\n");

    return 0;
}