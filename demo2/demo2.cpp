#include "test.h"
#include "../Log/Logger.h"

#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <exception>
#include <functional>
#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
//#include <event2/bufferevent_struct.h>
#//include <event2/bufferevent_struct.h>
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
#include <event2/http.h>

using event_base = struct event_base;
using event = struct event;
using evconnlistener = struct evconnlistener;
using bufferevent = struct bufferevent;
event_base * ev_base = nullptr;
bufferevent **g_bufev= nullptr;

std::condition_variable msg_cond;
std::mutex mute;
std::atomic<bool> terminateThread(false);



using sendMsgCallback = std::function<void(*)(bufferevent*)>;

void sendMsg_cb(bufferevent** pbufev)
{
    std::string str;
    LOG(LogLevel::DEBUG, "enter input function");
    while(!terminateThread.load(std::memory_order_acquire))
    {
        LOG(LogLevel::DEBUG, "block input loop");
        std::cin >> str;
        //if(bufferevent)
        std::cout << str << std::endl;
        //force terminating will tirggers core dump.

        str.append("\n");
        if(*pbufev)
        {
            LOG(LogLevel::DEBUG,"send");
            bufferevent_write(*pbufev, str.c_str(), str.size());
        }
        str.clear();
    }
    LOG(LogLevel::DEBUG, "termindate input thread");
    
}

static const char MESSAGE[] = "Hello, thanks for using libevent!";

static void W_cb(bufferevent* bufev, void* args);
static void R_cb(bufferevent* bufev, void* args);

static void listenConnection_cb(evconnlistener*, evutil_socket_t,
    sockaddr*, int, void*);
static void conn_eventcb(bufferevent*, short, void*);
static void signal_cb(evutil_socket_t, short, void*);

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
    *g_bufev = bufev;
    
    bufferevent_setcb(bufev, R_cb, W_cb, conn_eventcb, NULL);
    if(bufferevent_enable(bufev, EV_WRITE | EV_READ) == -1 )
    {
        LOG(LogLevel::DEBUG, "cant read!");
    }

    //bufferevent_write(bufev, MESSAGE, strlen(MESSAGE));

    //bufferevent_read_buffer(struct bufferevent *bufev, struct evbuffer *buf)
    //bufferevent_read(struct bufferevent *bufev, void *data, size_t size);
    //event* event = event_new(ev_base, newfd, EV_PERSIST | EV_READ | EV_WRITE, RW_cb, NULL);

    //event_add(event, NULL);

}
static void R_cb(bufferevent* bufev, void *userdata)
{
    LOG(LogLevel::DEBUG, "read callback!");
    struct evbuffer* input = bufferevent_get_input(bufev);
    size_t len = evbuffer_get_length(input);

    char data[len];
    evbuffer_remove(input, data, len);
    std::string str(data);
    str.resize(len - 1);
    LOG(LogLevel::DEBUG,"received MSG: " + str);
    LOG(LogLevel::DEBUG, "wocao");

    
    if(*g_bufev != bufev)
        *g_bufev = bufev;
    //isSend = true;
    //msg_cond.notify_one();
    //str.clear();
    //std::cin >> str;
    //bufferevent_write(bufev, str.c_str(), str.size());
}

static void W_cb(bufferevent* bufev, void *userdata)
{
    LOG(LogLevel::DEBUG, "welcome to w_cb!");
    /*struct evbuffer* output = bufferevent_get_output(bufev);
    if(evbuffer_get_length(output) == 0)
    {
        LOG(LogLevel::DEBUG, "flushed answer!");
        bufferevent_free(bufev);
    }
    */
}

static void conn_eventcb(bufferevent* bufev, short events, void* userdata)
{
    LOG(LogLevel::DEBUG, "call this!");
    if(events & BEV_EVENT_EOF)
    {
        LOG(LogLevel::DEBUG, "connection closed!");
    }else if (events & BEV_EVENT_ERROR)
    {
        LOG(LogLevel::ERROR,"connection GG!");
        strerror(errno);
    }

    bufferevent_free(bufev);
    bufev = nullptr;
}

static void signal_cb(evutil_socket_t sig, short events, void *userdata)
{
    event_base * base = static_cast<event_base*>(userdata);
    timeval delay = {.tv_sec = 2, .tv_usec = 0};

    LOG(LogLevel::DEBUG, "Caught an interrupt signal! ");
    terminateThread.store(true, std::memory_order_release);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    event_base_loopexit(base, &delay);
}
// struct threadUnit
// {
//     std::thread *gets_thread;
//     event_base* e_base;
// };

int Dtest2(){
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
    listener.sin_port = htons(13055);

    evconnlistener * evlistener =  evconnlistener_new_bind(ev_base, listenConnection_cb, (void*)ev_base,
                            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                            (struct sockaddr*)&listener,
                            sizeof(listener));
    // set callback when new incoming client connection, the event_base passed listen callback
    // via void* pointer.

    event *signal_event = evsignal_new(ev_base,SIGINT, signal_cb, (void*)ev_base);
    //event * ev = event_new(ev_base, -1, EV_PERSIST | EV_READ, listenConnection_cb, NULL);
    if(!signal_event || event_add(signal_event, NULL)<0)
    {
        LOG(LogLevel::DEBUG, "could not creat signal event!");
    }
    g_bufev = new bufferevent*;
    std::thread th(sendMsg_cb, g_bufev);
    th.detach();
    event_base_dispatch(ev_base);

    
    
    evconnlistener_free(evlistener);
    th.join();
    event_free(signal_event);

    event_base_free(ev_base);
    LOG(LogLevel::DEBUG, "exit");
    

    return 0;
}