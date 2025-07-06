#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h> // Unix socket

#define SOCKET_PATH "/tmp/my_socket"
#define BUFFER_SIZE 256

int main()
{
    int server_sock, client_sock;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0); // AF_UNIX -> a socket for unix domain  创建一个通信的端点
    if (server_sock == -1)
    {
        perror("socket created fail");
        return 1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) // 绑定到文件系统路径
    {
        perror("bind failed");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, 5) == -1) // 开始监听
    {
        perror("listen failed");
        close(server_sock);
        unlink(SOCKET_PATH);
        return 1;
    }

    printf("Server: Started, listening the path [%s]\n", SOCKET_PATH);

    client_len = sizeof(struct sockaddr_un);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len); // 接收连接 -> 阻塞
    if (client_sock == -1)
    {
        perror("accept failed");
        close(server_sock);
        unlink(SOCKET_PATH);
        return 1;
    }

    printf("Server: accept the connection from the client\n");

    ssize_t bytes_read;
    while ((bytes_read = read(client_sock, buffer, BUFFER_SIZE - 1)) > 0) // 读取数据 -> 阻塞
    {
        buffer[bytes_read] = '\0';
        printf("Server: received the message -> %s\n", buffer);
    }

    if (bytes_read == 0)
    {
        printf("Server: client has close the connection\n");
    }
    else
    {
        perror("read failed");
    }

    close(client_sock);
    close(server_sock);
    unlink(SOCKET_PATH);

    printf("Server close....\n");
    return 0;
}