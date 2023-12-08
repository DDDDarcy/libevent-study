#include "test.h"
#include "../Log/Logger.h"
#include <event.h>
#include <event2/event.h>
#include <event2/event_compat.h>
#include <openssl/opensslconf.h>
#include <iostream>
#include <event2/http.h>
#include <event2/event_struct.h>
#include <event2/listener.h>

#define ANY_IP "0.0.0.0"
#define PORT_ 8999

Demo2::Demo2(){  
    LOG(LogLevel::DEBUG, "create Demo2!~");
}

Demo2::~Demo2(){
    LOG(LogLevel::DEBUG, "destory Demo2!~");
}

bool Demo2::Init()
{
    using evhttp = struct evhttp;
    using event_base = struct event_base;
    using evhttp_bound_socket = struct evhttp_bound_socket;
    using evconnlistener = struct evconnlistener;

    event_base *eb = event_base_new();
    evhttp* server = evhttp_new(eb);
    evconnlistener *evlistener = NULL;
    evhttp_bound_socket* evsocket = NULL;
    LOG(LogLevel::DEBUG, "Libevent gogog1");
    evsocket = evhttp_bind_socket_with_handle(server, ANY_IP, PORT_); 
    LOG(LogLevel::DEBUG, "Libevent gogog");
    if(!evsocket)
    {
        LOG(LogLevel::DEBUG, "evhttp bind socket failure!");
    }
    LOG(LogLevel::DEBUG, "Libevent start!");
    evlistener = evhttp_bound_socket_get_listener(evsocket);

    //epoll add operation
    //evhttp_accept_socket(server, 3444);
    
    //evhttp_bound_set_bevcb(struct evhttp_bound_socket *bound, struct bufferevent *(*cb)(struct event_base *, void *), void *cbarg)
    event_base_dispatch(eb);

    

    
    return true;
}

bool Demo2::InitHttp()
{

    return true;
}