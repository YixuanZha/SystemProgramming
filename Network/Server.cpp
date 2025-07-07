#include "Mysocket.h"
#include <thread>

void HandleClient(MySocket client_socket)
{
    char buffer[BUFFERSIZE];
    while(true)
    {
        ssize_t len = client_socket.Read(buffer,BUFFERSIZE);
        if(len <= 0)
        {
            break;
        }
        buffer[len] = '\0';
        std::cout << "Receive the message: " << buffer << std::endl;
        client_socket.Write(buffer,len);
    }
    std::cout << "Client handle success" << std::endl;
}

int main()
{
    MySocket server;
    if(!server.CreateSocket("0.0.0.0",8080))
    {
        return 1;
    }
    if(!server.Bind())
    {
        return 1;
    }
    if(!server.Listen())
    {
        return 1;
    }
    std::cout << "Server is ready, waiting for the connection of clients" << std::endl;
    while(true)
    {
        MySocket client = server.Accept();
        // std::thread client_thread(HandleClient,client);
        // client_thread.detach();
        HandleClient(client);
    }
    return 0;
}