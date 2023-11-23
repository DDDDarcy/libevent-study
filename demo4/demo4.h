#pragma once
#include "server.h"
#include <memory>



class Demo4
{
public:
    Demo4();
    ~Demo4();
    void test_serverStart();
private:
    std::shared_ptr<Server> myserver;
};