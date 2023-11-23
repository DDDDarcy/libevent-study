#include "../Log/Logger.h"
#include"server.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdlib>
#include <memory>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>





Server::Server()
{
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8999);
    server_addr.sin_family = AF_INET;
    is_init_addr = true;
}

void Server::Socket()
{
    if(is_init_addr)
    {
        server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEPORT, nullptr, 0);
        is_init_socket = true;
    }else 
    {
        LOG(LogLevel::DEBUG, "server_sock_addr no init!");
        exit(0);
    }
}

void Server::Listen()
{
    if(is_init_socket)
    {
        ::bind(server_sock_fd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr));

        if( listen(server_sock_fd, 5) < 0)
        {
            LOG(LogLevel::DEBUG, "server listen fail!");
            exit(0);
        }

        is_init_listen = true;
    }else {
        LOG(LogLevel::DEBUG, "is_init_socket no init!");
        exit(0);
    }
    
}
void Server::Accept()
{   
    if(is_init_listen)
    {
        while(!is_stop){
            socklen_t len;
            std::shared_ptr<struct sockaddr_in> client = std::make_shared<struct sockaddr_in>();
            int fd = accept(server_sock_fd, (struct sockaddr*)client.get(), &len);

            if(fd < 0)
            {
                LOG(LogLevel::DEBUG, "accept fail!");
                exit(0);
            }
            
            
            std::string str(inet_ntoa(client->sin_addr));
            std::cout << str << std::endl;
            LOG(LogLevel::DEBUG, std::string("client connected, ip : ")+ str);
        }
    }
    close(server_sock_fd);
    
}
Server::~Server()
{
    
}


void Server::ServerRunLoop()
{
    Socket();
    Listen();
    Accept();
}