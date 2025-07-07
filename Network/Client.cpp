#include "Mysocket.h"

int main()
{
    MySocket client;
    if (!client.CreateSocket("127.0.0.1", 8080))
    {
        return -1;
    }

    if (!client.Connect())
    {
        return -1;
    }

    std::cout << "Enter the word you want to send" << std::endl;
    char send_buffer[BUFFERSIZE];
    char recv_buffer[BUFFERSIZE];

    while (true)
    {
        std::cout << "> ";

        if (fgets(send_buffer, BUFFERSIZE, stdin) == NULL)
        {
            break;
        }

        client.Write(send_buffer, strlen(send_buffer));

        if (strncmp(send_buffer, "quit", 4) == 0)
        {
            break;
        }

        ssize_t len = client.Read(recv_buffer, BUFFERSIZE - 1);
        if (len <= 0)
        {
            std::cout << "Server has stop the connection" << std::endl;
        }
        recv_buffer[len] = '\0';
        std::cout << "Receive the message: " << recv_buffer << std::endl;
    }
    std::cout << "Client is stopping...." << std::endl;
    return 0;
}