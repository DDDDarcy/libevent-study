#pragma once


#include <cstddef>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>

//using socketaddr_in = struct sockaddr_in;

struct socket_data{
    int sockfd;
    struct sockaddr_in *sockaddr;

};
class Server
{
public:
    Server(){}
    void initServer();
    void start();
private:
    int listenfd;
    int clientfd;

    struct sockaddr_in listenSock; 
    struct sockaddr_in clientSock;

};
