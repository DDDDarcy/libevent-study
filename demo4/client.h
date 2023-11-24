#include <memory>
#include<sys/socket.h>
#include <unordered_map>
#include <mutex>

class Client
{
public:
    Client();
    ~Client();

    
    void push(std::pair<int, std::shared_ptr<struct sockaddr_in>>);
    bool remove(int);
private:
    std::unordered_map<int, std::shared_ptr<struct sockaddr_in>> clientfds;
    std::mutex mutex;
};