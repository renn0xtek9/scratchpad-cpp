#include <iostream>
#include <cstring>
#include <csignal>

#include "arguments_parsing.h"
void usage(char bin_name[])
{
    IpAndPortConfig default_config;
    std::cout << "Usage: " << bin_name << " --port <port> --ip <ip>" << std::endl;
    std::cout << "Default port: " << default_config.port << std::endl;
    std::cout << "Default IP: " << default_config.ip << std::endl;

    exit(0);
}

IpAndPortConfig get_ip_and_port_from_arguments(int argc, char *argv[])
{
    IpAndPortConfig config;
    if (argc == 1)
    {
        return config;
    }
    if (argc > 5)
    {
        std::cerr << "Error: Too many arguments." << std::endl;
        usage(argv[0]);
    }

    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            usage(argv[0]);
        }
        else if (std::string(argv[i]) == "-p" || std::string(argv[i]) == "--port")
        {
            if (i + 1 < argc)
            {
                config.port = std::stoi(argv[++i]);
            }
            else
            {
                std::cerr << "Error: -p/--port option requires an argument." << std::endl;
                usage(argv[0]);
            }
        }
        else if (std::string(argv[i]) == "-i" || std::string(argv[i]) == "--ip")
        {
            if (i + 1 < argc)
            {
                config.ip = argv[++i];
            }
            else
            {
                std::cerr << "Error: -i/--ip option requires an argument." << std::endl;
                usage(argv[0]);
            }
        }
        else if (std::string(argv[i]) == "--port")
        {
            if (i + 1 < argc)
            {
                config.port = std::stoi(argv[++i]);
            }
            else
            {
                std::cerr << "Error: --port option requires an argument." << std::endl;
                usage(argv[0]);
            }
        }
        else if (std::string(argv[i]) == "--ip")
        {
            if (i + 1 < argc)
            {
                config.ip = argv[++i];
            }
            else
            {
                std::cerr << "Error: --ip option requires an argument." << std::endl;
                usage(argv[0]);
            }
        }
        else
        {
            std::cerr << "Error: Unknown option " << argv[i] << std::endl;
            usage(argv[0]);
        }
    }
    return config;
}
