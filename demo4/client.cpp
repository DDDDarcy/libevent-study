#include "client.h"
#include "../Log/Logger.h"

#include <algorithm>
#include <mutex>
#include <unistd.h>
#include <utility>

Client::Client()
{

}

Client::~Client()
{
    for(auto &x : clientfds)
    {
        close(x.first);
    }
}



void Client::push(std::pair<int, std::shared_ptr<struct sockaddr_in>> client)
{
    
    int nums = clientfds.size();
    auto it = std::find_if(clientfds.begin(), clientfds.end(), [&](const auto &x){
        return x.first == client.first;
    });

    std::lock_guard<std::mutex> lock(mutex);
    if( nums == clientfds.size() && it == clientfds.end() )
    {
        //To do vaild push 
        LOG(LogLevel::DEBUG, "can push");
        clientfds.insert(client);
    }else {
        LOG(LogLevel::ERROR, "can not push");
        return;
    }
}

bool Client::remove(int fd)
{
    int rfd;
    {
        std::lock_guard<std::mutex> lock(mutex);
        rfd = clientfds.erase(fd);
    }
    if(rfd == fd)
    {
        LOG(LogLevel::DEBUG, "remove valid");
        return true;
    }else
    {
        LOG(LogLevel::DEBUG, "remove fail");
        return false;
    }

}