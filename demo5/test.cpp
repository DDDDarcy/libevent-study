#include "test.h"
#include <event2/http.h>
#include <event2/http_struct.h>



#include <iostream>

Demo5::Demo5()
{
    std::cout << "Hello Client!" << std::endl;
    int a = 5;
}

Demo5::~Demo5()
{

}

bool Demo5::Init()
{

    return true;
}

bool Demo5::InitHttp()
{

    return true;
}