#ifndef MY_SOCKET_H_
#define MY_SOCKET_H_

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 256

class MySocket
{
public:
    MySocket();
    ~MySocket();

    bool CreateSocket(const std::string &ip, const int port);
    bool SetNonBlock();
    bool Bind();
    bool Listen();
    MySocket Accept();
    ssize_t Read(char *buffer, size_t size);
    ssize_t Write(char *buffer, size_t size);
    bool Connect();
    void Close();

private:
    explicit MySocket(int fd);
    std::string m_ip;
    int m_port;
    int m_socket_fd;
    struct sockaddr_in server_addr;
};

#endif