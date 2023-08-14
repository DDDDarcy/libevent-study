#include "test.h"
#include "../Log/Logger.h"

#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <event2/bufferevent_struct.h>
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

static const char MESSAGE[] = "Hello, thanks for using libevent!";

static void W_cb(bufferevent* bufev, void* args);
static void R_cb(bufferevent* bufev, void* args);

static void listenConnection_cb(evconnlistener*, evutil_socket_t,
    sockaddr*, int, void*);
static void conn_eventcb(bufferevent*, short, void*);
static void siginal_cb(evutil_socket_t, short, void*);

static void listenConnection_cb(
    evconnlistener* listener, 
    evutil_socket_t newfd, 
    sockaddr* sourecaddr, 
    int socklen, 
    void * userdata)
{
    LOG(LogLevel::DEBUG, "call new connection cb");
    event_base * base = static_cast<event_base*>(userdata);
    bufferevent * bufev;
    
    bufev = bufferevent_socket_new(base, newfd,BEV_OPT_CLOSE_ON_FREE);
    if(!bufev)
    {
        LOG(LogLevel::DEBUG, "Error constructing bufferevent!");
    
    }

    bufferevent_setcb(bufev, R_cb, W_cb, conn_eventcb, NULL);
    bufferevent_enable(bufev, EV_WRITE | EV_READ);

    bufferevent_write(bufev, MESSAGE, strlen(MESSAGE));
    //event* event = event_new(ev_base, newfd, EV_PERSIST | EV_READ | EV_WRITE, RW_cb, NULL);

    //event_add(event, NULL);

}

static void W_cb(bufferevent* bufev, void *userdata)
{
    struct evbuffer* output = bufferevent_get_output(bufev);
    if(evbuffer_get_length(output) == 0)
    {
        LOG(LogLevel::DEBUG, "flushed answer!");
        bufferevent_free(bufev);
    }
}

static void conn_eventcb(bufferevent* bufev, short events, void* userdata)
{
    if(events & BEV_EVENT_EOF)
    {
        LOG(LogLevel::DEBUG, "connection closed!");
    }else if (events & BEV_EVENT_ERROR)
    {
        LOG(LogLevel::ERROR,"connection GG!");
        strerror(errno);
    }

    bufferevent_free(bufev);
}

static void signal_cb(evutil_socket_t sig, short events, void *userdata)
{
    event_base * base = static_cast<event_base*>(userdata);
    timeval delay = {.tv_sec = 2, .tv_usec = 0};

    LOG(LogLevel::DEBUG, "Caught an interrupt signal!");

    event_base_loopexit(base, &delay);
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

    evconnlistener_free(evlistener);

    event_free(signal_event);

    event_base_free(ev_base);

    return 0;
}