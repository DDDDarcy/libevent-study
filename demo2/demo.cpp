#include "test.h"
#include <iostream>
#include "../Log/Logger.h"


int main(){
    Demo2 D2;
    LOG(LogLevel::DEBUG, "Helo !");

    D2.Init();
}