#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char buf[100] = {0};
    ssize_t byte_read;
    int flag;

    int fd = open("/dev/input/mouse1",O_RDONLY|O_NONBLOCK);
    if(fd < 0)
    {
        perror("Failed to open the file /dev/input/mouse1");
    }
    else
    {
        printf("Open the file sucessfully!\n");
    }

    flag = fcntl(STDIN_FILENO,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(STDIN_FILENO,F_SETFL,flag);

    while(1)
    {
        bzero(buf,sizeof(buf));
        read(fd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,strlen(buf));

        bzero(buf,sizeof(buf));
        byte_read = read(STDIN_FILENO,buf,sizeof(buf));
        buf[byte_read] = '\0';
        write(STDOUT_FILENO,buf,byte_read);
    }
    return 0;
}