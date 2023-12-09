#include "test.h"
#include <iostream>
#include "../Log/Logger.h"


int main(){
    Demo5 D5;
    LOG(LogLevel::DEBUG, "Helo !");

    D5.Init();
}