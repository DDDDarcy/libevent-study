#include"sock.h"


#include <cstring>
#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>






void Server::initServer()
{
    memset(&listenSock, 0, sizeof listenSock);
    listenSock.sin_addr.s_addr = htonl(INADDR_ANY);
    listenSock.sin_port = htons(13055);
    listenSock.sin_family = AF_INET;
   
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    ::bind(listenfd, (struct sockaddr*)(&listenSock), sizeof(listenSock));

    
}

void Server::start()
{
    std::cout << "go start" << std::endl;
    listen(listenfd, 1024);

    socklen_t len;
    std::cout << "block" << std::endl;
    clientfd = accept(listenfd, (struct sockaddr*)&clientSock, &len);
    std::cout << "accepted" << std::endl;
    char buf[1024] = "wo shou dao le";
    std::cout << "send data" << std::endl;
    send(clientfd, buf, sizeof(buf), 0);
    
    close(clientfd);

}
