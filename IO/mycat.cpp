#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h> // Standard I/O -> have buffer

#define BUFSIZE 4096

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    char *FileName = argv[1];
    int fd = open(FileName, O_RDONLY);
    // std::cout << fd << std::endl;

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    int n;
    char buf[BUFSIZE];

    while ((n = read(fd, buf, BUFSIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buf, n) != n)
        {
            perror("write error");
            close(fd);
            return 1;
        }
    }
    if (n == -1)
    {
        perror("read error");
        close(fd);
        return 1;
    }

    close(fd);

    return 0;
}