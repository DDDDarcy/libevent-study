#include "test.h"
#include "../Log/Logger.h"

#include <cstddef>
#include <cstring>
#include <signal.h>

#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using event_base = struct event_base;
using event = struct event;
using evconnlistener = struct evconnlistener;
using bufferevent = struct bufferevent;
event_base * ev_base = nullptr;



static void RW_cb(evutil_socket_t clientfd, short flags, void* args);
static void listenConnection_cb(evconnlistener*, evutil_socket_t,
    sockaddr*, int, void*);
static void conn_eventcb(bufferevent, short, void*);
static void siginal_cb(evutil_socket_t, short, void*);

static void listenConnection_cb(
    evconnlistener* listener, 
    evutil_socket_t newfd, 
    sockaddr* sourecaddr, 
    int socklen, 
    void * userdata)
{
    LOG(LogLevel::DEBUG, "call new connection cb");
    
    event* event = event_new(ev_base, newfd, EV_PERSIST | EV_READ | EV_WRITE, RW_cb, NULL);

    event_add(event, NULL);

}


int main(){
    Demo2 demo;
    LOG(LogLevel::DEBUG, "init libevent");
    
    ev_base = event_base_new();
    if(!ev_base)
    {
        LOG(DEBUG,"base failed!");
        return 1;
    }

    //event_base * ev = nullptr;
    sockaddr_in listener;
    memset(&listener, 0, sizeof listener);
    listener.sin_addr.s_addr = htonl(INADDR_ANY);
    listener.sin_family = AF_INET;
    listener.sin_port = htons(8888);

    evconnlistener * evlistener =  evconnlistener_new_bind(ev_base, listenConnection_cb, (void*)ev_base,
                            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                            (struct sockaddr*)&listener,
                            sizeof(listener));
    // set callback when new incoming client connection, the event_base passed listen callback
    // via void* pointer.

    event *signal_event = evsignal_new(ev_base,SIGINT, siginal_cb, (void*)ev_base);
    //event * ev = event_new(ev_base, -1, EV_PERSIST | EV_READ, listenConnection_cb, NULL);
    if(!signal_event || event_add(signal_event, NULL)<0)
    {
        LOG(LogLevel::DEBUG, "could not creat signal event!");
    }

    event_base_dispatch(ev_base);

    evconnlistener_free(listener);

    event_add(ev,NULL);

    evutil_socket_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ::bind(listen_fd,(struct sockaddr*)&listener, sizeof listener);

    listen(listen_fd, 1024);

    event_base_dispatch(ev_base);


    return 0;
}