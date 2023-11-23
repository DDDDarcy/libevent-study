#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>

class Server{
public:
    Server();
    ~Server();
    void ServerRunLoop();
private:
    sockaddr_in server_addr;
    int server_sock_fd;
    bool is_init_addr;
    bool is_init_socket;
    bool is_init_listen;
    bool is_stop;
    
    void Socket();
    void Listen();
    void Accept();
};
