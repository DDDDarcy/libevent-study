#include"demo3.h"
#include"sock.h"


Demo3::Demo3()
{
	server = std::make_shared<Server>();
}

void Demo3::start()
{
	server->initServer();
	server->start();
}
