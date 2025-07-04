#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <linux/input.h>

// poll使用数组来管理文件描述符，它的大小只受限于系统内存，解决了select最致命的FD_SETSIZE限制。
// struct pollfd的结构比位图操作（FD_ZERO, FD_SET等）更清晰易懂。
// 每次调用poll，仍然需要将整个fds数组从用户空间拷贝到内核空间，并且内核仍然需要线性遍历这个数组来检查每个FD的状态。

// //struct pollfd {
//     int   fd;         // 文件描述符 -> fds[1] = fd;
//     short events;     // 你感兴趣的事件 (输入) -> fds[1].event = PULLIN
//     short revents;    // 实际发生的事件 (由内核填充的输出) -> if(fds[1].revents && PULLIN) -> 有新的数据可以读取
// };

int main()
{
    int fd_mouse;
    int fd_keyboard;
    char keyboard_buf[101];

    const char *mouse_dev = "/dev/input/mouse2";
    fd_mouse = open(mouse_dev, O_RDONLY);

    if (fd_mouse < 0)
    {
        perror("Open the mouse failed");
        return 1;
    }

    fd_keyboard = STDIN_FILENO;

    // 这个数组只需要在循环外设置一次，因为poll不会修改它
    struct pollfd fds[2];

    fds[0].fd = fd_keyboard;
    fds[0].events = POLLIN; // POLLIN 表示关心“有数据可读”这一事件

    fds[1].fd = fd_mouse;
    fds[1].events = POLLIN;

    printf("Open the device successfully\n");

    while (1)
    {
        printf("Call poll() -> blocked\n");

        // 调用 poll() 函数，阻塞等待事件发生
        // 第一个参数是 pollfd 数组
        // 第二个参数是数组的大小
        // 第三个参数是超时时间（毫秒），-1表示永久等待
        int ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("poll() error");
            break;
            ;
        }

        if (fds[0].revents & POLLIN)
        {
            memset(keyboard_buf, 0, sizeof(keyboard_buf));
            ssize_t bytes_read = read(fd_keyboard, keyboard_buf, sizeof(keyboard_buf));
            if (bytes_read > 0)
            {
                printf("----Keyboard event---- Input is: %s", keyboard_buf);
            }
        }

        if (fds[1].revents & POLLIN)
        {
            struct input_event mouse_event;
            ssize_t bytes_read = read(fd_mouse, &mouse_event, sizeof(struct input_event));
            printf("----Mouse event----: Type: %d, Code: %d, Value: %d\n", mouse_event.type, mouse_event.code, mouse_event.value);
        }
    }

    close(fd_mouse);
    return 0;
}