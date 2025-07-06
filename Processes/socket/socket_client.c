#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_socket"
#define BUFFER_SIZE 256

int main()
{
    int client_sock;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    client_sock = socket(AF_UNIX,SOCK_STREAM,0); // 创建自己的套接字
    if(client_sock == -1)
    {
        perror("socket created failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path,SOCKET_PATH,sizeof(server_addr.sun_path) - 1);

    printf("Client: Try to connect to [%s]\n",SOCKET_PATH);

    if(connect(client_sock,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_un)) == -1) // 连接服务器
    {
        perror("connect failed: please make sure that the server is running");
        close(client_sock);
        return 1;
    }
    printf("Client: connected to the server success\n");

    printf("Please enter the word you want to send\n");
    while (1)
    {
        printf("> ");
        if(fgets(buffer,BUFFER_SIZE,stdin) == NULL)
        {
            break;
        }

        if(strncmp(buffer,"quit",4) == 0)
        {
            break;
        }

        if(write(client_sock,buffer,strlen(buffer)) == -1) // 发送数据
        {
            perror("write to socket failed");
            break;
        }
    }
    

    close(client_sock); // 关闭连接
    printf("Client: close the connection, exit.....\n");
    return 0;
}