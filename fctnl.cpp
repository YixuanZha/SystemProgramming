#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

class FCTNL
{
    public:
    void NonBlockRead()
    {
        Settingfalgs();
        sleep(5);
        ReadFromSTDInput();
    }

    private:
    int flags;
    char buffer[100];

    void Settingfalgs()
    {
        flags = fcntl(STDIN_FILENO,F_GETFL);
        if(flags == -1)
        {
            perror("fctnl F_GETFL failed");
            return;
        }

        flags |= O_NONBLOCK;

        if(fcntl(STDIN_FILENO,F_SETFL,flags) == -1)
        {
            perror("fctnl F_SETFL failed");
            return;
        }

        fprintf(stdout,"Standard output has been setted to non block\n");
        fprintf(stdout,"Please try to enter some contents and the press enter\n");
    }

    void ReadFromSTDInput()
    {
        ssize_t bytes_read = read(STDIN_FILENO,buffer,sizeof(buffer));
        if(bytes_read == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                fprintf(stdout, "Can not detected the input data, the program has not been blocked\n");
            }
            else
            {
                perror("read failed");
            }
        }
        else
        {
            buffer[bytes_read] = '\0';
            fprintf(stdout,"Read %ld bytes sucessfully: %s\n",bytes_read, buffer);
        }
    }
};

int main(int argc, char* argv[])
{
    FCTNL test;
    test.NonBlockRead();
    
}