#include "Mysocket.h"

MySocket::MySocket() : m_ip(""), m_port(0), m_socket_fd(-1)
{
}

MySocket::MySocket(int fd) : m_ip(""), m_port(0), m_socket_fd(fd)
{
}

MySocket::~MySocket()
{
    Close();
}

bool MySocket::CreateSocket(const std::string &ip, const int port)
{
    m_ip = ip;
    m_port = port;
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd == -1)
    {
        return false;
    }
    std::cout << "Create the socket success, the fd of the socket is: " << m_socket_fd << std::endl;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton() failed");
        close(m_socket_fd);
        m_socket_fd = -1;
        return false;
    }
    return true;
}

bool MySocket::SetNonBlock()
{
    int flag = fcntl(m_socket_fd, F_GETFL);
    if (flag < 0)
    {
        perror("fcntl cannot get flag");
        return false;
    }
    flag |= O_NONBLOCK;
    if(fcntl(m_socket_fd,F_SETFL,flag) == -1)
    {
        perror("failed set the flag");
        return false;
    }
    std::cout << "Set the fd to Non Block" << std::endl;
    return true;
}

bool MySocket::Bind()
{
    if (bind(m_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed");
        close(m_socket_fd);
        m_socket_fd = -1;
        return false;
    }
    std::cout << "Bind success" << std::endl;
    return true;
}

bool MySocket::Listen()
{
    if (listen(m_socket_fd, 1024) == -1)
    {
        perror("listen() failed");
        close(m_socket_fd);
        m_socket_fd = -1;
        return false;
    }
    std::cout << "Start listening" << std::endl;
    return true;
}

MySocket MySocket::Accept()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(sockaddr_in);
    int client_socket_fd = accept(m_socket_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket_fd < 0)
    {
        perror("Accept() failed");
        return MySocket(-1);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET6_ADDRSTRLEN);
    std::cout << "Receive new connection, from: " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

    return MySocket(client_socket_fd);
}

ssize_t MySocket::Read(char *buffer, size_t size)
{
    ssize_t bytes_read = read(m_socket_fd, buffer, size);
    if (bytes_read < 0)
    {
        perror("read() failed");
    }
    return bytes_read;
}

ssize_t MySocket::Write(char *buffer, size_t size)
{
    ssize_t bytes_written = write(m_socket_fd, buffer, size);
    if (bytes_written < 0)
    {
        perror("write() failed");
    }
    return bytes_written;
}

bool MySocket::Connect()
{
    if (connect(m_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connect failed");
        close(m_socket_fd);
        m_socket_fd = -1;
        return false;
    }
    return true;
}

void MySocket::Close()
{
    if (m_socket_fd != -1)
    {
        close(m_socket_fd);
        m_socket_fd = -1;
    }
}
