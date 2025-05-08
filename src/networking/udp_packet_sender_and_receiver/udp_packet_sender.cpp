#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <thread>
#include <chrono>

#include "arguments_parsing.h"
void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received. Exiting gracefully...\n";
    exit(signum);
}

int main(int argc, char *argv[])
{
    const IpAndPortConfig config = get_ip_and_port_from_arguments(argc, argv);
    std::signal(SIGINT, signalHandler);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        std::cerr << "Cannot create a socket.\n";
        return -1;
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(config.port);
    server.sin_addr.s_addr = inet_addr(config.ip.c_str());

    const std::string msg = "Dummy UDP Packet";

    while (true)
    {

        if (sendto(sockfd, msg.c_str(), msg.size() + 1, 0,
                   (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            std::cerr << "Error in sending the message.\n";
            return -1;
        }

        std::cout << "UDP packet sent to " << config.ip << " on port " << config.port << " successfully.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sockfd);
    return 0;
}
