// Some simple demos of lseek
// The core function of the lseek function is to set the exact ‘file offset’ associated with a file descriptor.
// SEEK_SET seek from the head of the file
// SEEK_CUR seek from the current position
// SEEK_END seek from the end of the file

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

class LseekTest
{
public:
    LseekTest()
    {
        my_fd = -1;
    }

    ~LseekTest()
    {
        if(my_fd == -1)
        {
            close(my_fd);
            fprintf(stdout,"The file discripter %d has been close\n",my_fd);
        }

        if(my_filename)
        {
            remove(my_filename);
        }
    }

    void CreateFileAndWriteContent(const char *FileName, const char *InitialContent)
    {
        my_filename = FileName;
        int fd = open(FileName, O_CREAT | O_RDWR | O_TRUNC);
        my_fd = fd;
        if (fd == -1)
        {
            perror("open or create file failed\n");
        }

        if (write(fd, InitialContent, strlen(InitialContent)) == -1)
        {
            HandleError("Failed to write the initial content");
        }
        fprintf(stdout, "Create the file success '%s', and write the content '%s'\n", FileName, InitialContent);
    }

    void SeekSetFromHead()
    {
        char buffer[100];
        fprintf(stdout,"-------SEEK_SET--------\n");
        if(lseek(my_fd,0,SEEK_SET) == -1)
        {
            HandleError("lseek (SEEK_SET) failed");
        }
        ssize_t byte_read = read(my_fd,buffer,4);
        if(byte_read == -1)
        {
            HandleError("read failed");
        }
        buffer[byte_read] = '\0';
        fprintf(stdout,"Read frome the head from the file, the content is: '%s'\n\n", buffer);
    }

    void SeekCurFromCurrentPosition()
    {
        char buffer[100];
        fprintf(stdout,"-------SEEK_CUR--------\n");
        if(lseek(my_fd,6,SEEK_CUR) == -1)
        {
            HandleError("lseej (SEEK_CUR) failed");
        }
        ssize_t bytes_read = read(my_fd, buffer,3);
        if(bytes_read == -1)
        {
            HandleError("failed to read");
        }
        buffer[bytes_read] = '\0';
        fprintf(stdout, "Move 6 bytes forward from current position, then read 3 bytes, the content is: '%s'\n\n",buffer);
    }

    void SeekENDGetTheSize()
    {
        fprintf(stdout,"-------SEEK_END--------\n");
        off_t file_size = lseek(my_fd,0,SEEK_END);
        if(file_size == -1)
        {
            HandleError("lseek (SEEK_END) failed");
        }
        fprintf(stdout,"The size of file '%s' is: %lld bytes\n\n",my_filename,(long long) file_size);
    }

    void SeekSetOverwrite()
    {
        fprintf(stdout,"-------Overwrite file content--------\n");        
        if(lseek(my_fd,5,SEEK_SET) == -1)
        {
            HandleError("Overwrite the file failed (before)");
        }

        if(write(my_fd,"---",3) == -1)
        {
            HandleError("Overwrite the file failed");
        }
        fprintf(stdout,"Write '---' to the fifth byte of the file\n");
    }

private:
    int my_fd;
    const char* my_filename;

    void HandleError(const char *msg)
    {
        perror(msg);
        exit(1);
    }
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: <filename> <content> \n");
    }

    const char *FileName = argv[1];
    const char *InitialContent = argv[2];

    LseekTest lseektest;
    lseektest.CreateFileAndWriteContent(FileName,InitialContent);
    lseektest.SeekSetFromHead();
    lseektest.SeekCurFromCurrentPosition();
    lseektest.SeekENDGetTheSize();
    lseektest.SeekSetOverwrite();
}