#include "test.h"
#include "event.h"
#include "../Log/Logger.h"

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>


Demo1::Demo1(){
    std::cout << "create demo1!" << std::endl;
    
}

bool Demo1::init(){
    LOG(LogLevel::DEBUG, "init event.");
    base_ = event_base_new();
    struct sockaddr_in sin;
    struct event *ev_listener;

    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(8888);

    ev_listener = event_new(base_, -1, EV_READ| EV_PERSIST, Demo1::echo_msg, this);
    event_add(ev_listener, NULL);

    evutil_socket_t listener_fd = socket(AF_INET, SOCK_STREAM, 0);
    ::bind(listener_fd, (struct sockaddr*)&sin, sizeof sin);

    listen(listener_fd, 10);

    event_base_dispatch(base_);

    return true;


}

void Demo1::echo_msg(evutil_socket_t fd, short events, void *arg){
    char buffer[1024];
    struct event *ev = (struct event*)arg;
    int len = recv(fd,buffer, sizeof buffer, 0);
    if(len <= 0)
    {
        event_free(ev);
        ::close(fd);
    }else {
        send(fd, buffer, sizeof buffer, 0);
    }

}


Demo1::~Demo1(){
    std::cout << "destory demo1!" << std::endl;
}

