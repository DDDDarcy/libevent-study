#include"demo4.h"
#include <memory>
#include "../Log/Logger.h"

Demo4::Demo4()
    :myserver(std::make_shared<Server>())
{
    LOG(LogLevel::DEBUG, "welcome to open Demo4!");
}

Demo4::~Demo4()
{

}

void Demo4::test_serverStart()
{
    LOG(LogLevel::DEBUG, "test server Start until accept syscall!");
    myserver->ServerRunLoop();
}