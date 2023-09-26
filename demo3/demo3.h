#pragma once
#include<memory>

class Server;
class Demo3{
public:
    Demo3();
    void start();
private:
    std::shared_ptr<Server> server;
};
