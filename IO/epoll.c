// epoll的设计完全不同 -> 通过epoll_ctl将需要监视的FD一次性地注册到内核的一个特殊数据结构（通常是红黑树）中。
// 内核操作：内核会维护一个“就绪列表”（Ready List）。当某个连接有数据到达时，内核会通过回调机制，主动地将这个就绪的FD放入就绪列表中。这个过程是事件驱动的，非常高效。
// epoll_wait 的作用：调用epoll_wait时，它不需要遍历所有FD。它仅仅是检查一下那个“就绪列表”是否为空。如果不为空，它就把列表里的内容直接返回给你。


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <linux/input.h>

#define MAX_EVENT 2

int main()
{
    int fd_mouse;
    int fd_keyboard;
    int epoll_fd;
    struct epoll_event event, events[MAX_EVENT];

    const char *mouse_device = "/dev/input/mouse2";
    fd_mouse = open(mouse_device, O_RDONLY | O_NONBLOCK);

    if (fd_mouse < 0)
    {
        perror("Failed to open the Mouse file");
        return 1;
    }

    fd_keyboard = STDIN_FILENO;

    epoll_fd = epoll_create1(0); // 创建一个 epoll 实例 -> 操作系统内核会在自己的内存里创建一个专属的、高效的监视列表（通常是用红黑树实现的）
    if (epoll_fd == -1)
    {
        perror("epoll created1 failed");
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = fd_keyboard;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_keyboard, &event) == -1)
    {
        perror("epoll_ctl failed to add keyboard");
        close(fd_keyboard);
        close(epoll_fd);
        return 1;
    }

    event.data.fd = fd_mouse;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_mouse, &event) == -1)
    {
        perror("epoll_ctl failed to add mouse");
        close(fd_mouse);
        close(epoll_fd);
        return 1;
    }

    printf("Open the Device sucefully, wait for the input of the keyboard and mouse\n");

    while (1)
    {
        printf("Call epoll(), blocked to wait .......\n");

        int num_event = epoll_wait(epoll_fd, events, MAX_EVENT, -1);
        if (num_event < 0)
        {
            perror("epoll_wait() failed");
            break;
        }

        for (int i = 0; i < num_event; i++)
        {
            if (events[i].data.fd == fd_keyboard)
            {
                char keyboard_buf[101];
                memset(keyboard_buf, 0, sizeof(keyboard_buf));
                ssize_t bytes_read = read(fd_keyboard, keyboard_buf, sizeof(keyboard_buf) - 1);
                if (bytes_read > 0)
                {
                    if (strncmp(keyboard_buf, "quit", 4) == 0)
                    {
                        printf("QUIT............\n");
                        goto cleanup;
                    }
                    printf("----Keyboard event-----The input value is: %s\n\n", keyboard_buf);
                }
            }

            else if (events[i].data.fd == fd_mouse)
            {
                struct input_event mouse_event;
                ssize_t bytes_read = read(fd_mouse, &mouse_event, sizeof(mouse_event));
                if (bytes_read == sizeof(struct input_event))
                {
                    printf("----Mouse event----: Type: %d, Code: %d, Value: %d\n\n", mouse_event.type, mouse_event.code, mouse_event.value);
                }
            }
        }
    }

cleanup:
    close(fd_mouse);
    close(epoll_fd);
    return 0;
}