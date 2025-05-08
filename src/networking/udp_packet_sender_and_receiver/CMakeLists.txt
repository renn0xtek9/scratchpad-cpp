my_add_executable(NAME udp_packet_receiver
    SRCS udp_packet_receiver.cpp arguments_parsing.cpp
    CPP_STANDARD 20
)

my_add_executable(NAME udp_packet_sender
    SRCS udp_packet_sender.cpp arguments_parsing.cpp
    CPP_STANDARD 20 
)
