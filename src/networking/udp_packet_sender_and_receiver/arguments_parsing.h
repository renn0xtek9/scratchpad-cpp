struct IpAndPortConfig {
  int port{8080};
  std::string ip{"127.0.0.1"};
};
void usage(char bin_name[]);
IpAndPortConfig get_ip_and_port_from_arguments(int argc, char* argv[]);
