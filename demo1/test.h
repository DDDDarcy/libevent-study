#pragma once

class Demo1{
    public:
        Demo1();
        bool init();
        static void echo_msg(int, short, void *);
        ~Demo1();
    private:
        struct event_base *base_;
};