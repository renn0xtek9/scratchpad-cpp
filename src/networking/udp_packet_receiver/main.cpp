// ReceiverUDPPacket.cpp

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>

constexpr int PORT = 8080;         // Set your port
constexpr char IP[] = "127.0.0.1"; // Set your IP

void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received. Exiting gracefully...\n";
    exit(signum);
}

int main()
{
    std::signal(SIGINT, signalHandler);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        std::cerr << "Cannot create a socket.\n";
        return -1;
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(IP);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        std::cerr << "Error in binding the socket to the IP/port.\n";
        return -1;
    }

    while (true)
    {

        std::cout << "Waiting for a message on " << IP << ":" << PORT << "...\n";

        char msg[1024];
        socklen_t serverLen = sizeof(server);

        if (recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&server, &serverLen) == -1)
        {
            std::cerr << "Error in receiving the message.\n";
            return -1;
        }

        std::cout << "Received a message: " << msg << "\n";
    }

    close(sockfd);
    return 0;
}
