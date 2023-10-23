#include<vector>
#include<iostream>
//#include"demo3.h"
#include"msg.pb.h"
int main()
{
    msg::MyMessage *message = new msg::MyMessage;
    message->set_id(1);
    message->set_name("Hello");
    std::cout << message->name() << std::endl; 
    // Demo3 d3;
    // std::cout << "complete demo3 " << std::endl;
    // d3.start();
    return 0;
}
