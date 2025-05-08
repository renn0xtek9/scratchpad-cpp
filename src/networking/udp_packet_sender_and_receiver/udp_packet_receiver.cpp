#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>

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

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        std::cerr << "Error in binding the socket to the IP/port.\n";
        return -1;
    }

    while (true)
    {

        std::cout << "Waiting for a message on " << config.ip << ":" << config.port << "...\n";

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
