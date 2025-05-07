// SendUDPPacket.cpp

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <thread>
#include <chrono>

constexpr int PORT = 8080;         // Set your destination port
constexpr char IP[] = "127.0.0.1"; // Set your destination IP

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

    const std::string msg = "Dummy UDP Packet";

    while (true)
    {

        if (sendto(sockfd, msg.c_str(), msg.size() + 1, 0,
                   (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            std::cerr << "Error in sending the message.\n";
            return -1;
        }

        std::cout << "UDP packet sent to " << IP << " on port " << PORT << " successfully.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sockfd);
    return 0;
}
