#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <linux/input.h>

// 大小限制：位图的大小是固定的，由一个名为FD_SETSIZE的宏决定 -> select默认最多只能同时监视1024个文件描述符
// 每次调用select，都需要把整个fd_set（无论里面有多少个有效的FD）从用户空间完整地拷贝到内核空间 -> 当监视的FD数量很大时，效率低下

#define max(a,b) ((a) > (b)) ? (a) : (b)

int main()
{
    int fd_mouse;
    int fd_keyboard;
    char keyboard[101];

    const char* mouse_dev = "/dev/input/mouse2";
    fd_mouse = open(mouse_dev,O_RDONLY);
    if(fd_mouse < 0)
    {
        perror("open the device of mouse failed");
        return 1;
    }

    fd_keyboard = STDIN_FILENO;

    while(1)
    {
        fd_set read_fds; // fd_set -> file descripter set -> 存放一组你希望监视的文件描述符（FD）。
        // fd_set 本质上是一个位图（Bitmap），通常是一个固定大小的整数数组
        int ret;

        FD_ZERO(&read_fds); // 清空一个fd_set集合，即将所有比特位都设置为0。 -> 在每次调用select之前，都必须用它来初始化你的集合。
        FD_SET(fd_keyboard,&read_fds); // 将一个指定的文件描述符fd添加到集合set中（即将对应的比特位置为1）。
        FD_SET(fd_mouse,&read_fds);

        int nfds = max(fd_keyboard,fd_mouse) + 1;

        printf("Call select(), blocked\n");

        ret = select(nfds,&read_fds,NULL,NULL,NULL);

        if(ret < 0)
        {
            perror("Select error");
            break;
        }

        if(FD_ISSET(fd_mouse,&read_fds)) // 检查一个文件描述符fd是否仍然在集合set中 -> 在select函数返回后，用它来遍历并找出具体是哪个FD准备好了。
        {
            struct input_event mouse_event;
            ssize_t bytes_read = read(fd_mouse,&mouse_event,sizeof(struct input_event));

            if(bytes_read > 0)
            {
                printf("----Mouse event----: Type: %d, Code: %d, Value: %d\n",mouse_event.type,mouse_event.code,mouse_event.value);
            }
        }

        if(FD_ISSET(fd_keyboard,&read_fds))
        {
            memset(keyboard,0,sizeof(keyboard));
            ssize_t bytes_read = read(fd_keyboard,&keyboard,sizeof(keyboard) - 1);

            if(bytes_read > 0)
            {
                printf("-----Keyboard event-----: %s",keyboard);
            }
        }
    }
    close(fd_mouse);
    return 0;
}