// mmap的使用
// 本质上是把磁盘上的文件与虚拟地址空间建立映射
// void * mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset)
// char* mapped_mem = (char*) mmap(NULL,file_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0)
// addr = NULL -> 由操作系统自动选择映射地址
// len = file_size -> 文件的大小
// prot = PROT_READ | PROT_WRITE -> 映射区域可读可写
// flags = MAP_SHARED -> 对映射内存的修改写会文件，并对其他映射该文件的进程可见
// fd -> 映射文件的文件描述符
// offset -> 映射的起始偏移量

// int munmap(void *__addr, size_t __len) -> 解除映射
// addr -> mapped_men -> 是一个指针变量，存储的是虚拟内存的地址
// len -> file_size

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
    const char *path = "test_mmap.txt";
    const char *text = "This is a test text for mmap file";
    int fd;
    char *mapped_mem;
    struct stat file_stat;

    fd = open(path, O_RDWR | O_CREAT | O_TRUNC); // O_RDWR -> 以读写方式打开 O_CREAT -> 如果文件不存在则创建 O_TRUNC -> 存在内容则清除
    if (fd == -1)
    {
        perror("failed to open the file");
        return 1;
    }

    if (write(fd, text, strlen(text)) != strlen(text))
    {
        perror("Failed to write to the file");
        return 1;
    }

    printf("1. Test file %s has been created, the inital content is: %s \n", path, text);

    if (fstat(fd, &file_stat) == -1)
    {
        perror("failed to get the stat of the file");
        close(fd);
        return 1;
    }

    size_t file_size = file_stat.st_size;
    printf("2. The size of the test file is: %ld\n", file_size);

    // 调用 mmap()
    // NULL: 让内核自动选择映射的起始地址
    // file_size: 映射的长度
    // PROT_READ | PROT_WRITE: 映射区域可读可写
    // MAP_SHARED: 对映射内存的修改会写回到文件，并对其他映射该文件的进程可见
    // fd: 要映射的文件的文件描述符
    // 0: 映射的起始偏移量
    mapped_mem = (char*) mmap(NULL,file_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0); 
    if(mapped_mem == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    close(fd);
    printf("3. The file has mapped to the address %s successfully\n",mapped_mem);

    // Can use pointer to modify the value of memory
    mapped_mem[0] = 'A';
    mapped_mem[1] = 'B';
    mapped_mem[2] = 'C';
    printf("4. Afer modify, the content is: %s\n",mapped_mem);

    if(munmap(mapped_mem,file_size) == -1)
    {
        perror("munmap failed");
        return 1;
    }

    printf("5. Use trafitional way to open the file\n");
    fd = open(path,O_RDONLY);
    if(fd == -1)
    {
        perror("failed to open the file with traditional way");
        return 1;
    }

    char buffer[100] = {0};
    if(read(fd,buffer,sizeof(buffer) -1) == -1)
    {
        perror("Failed to read the file");
        close(fd);
        return 1;
    }

    write(STDOUT_FILENO,buffer,sizeof(buffer)); // Can modify the original file !!!!

    close(fd);

    return 0;
}