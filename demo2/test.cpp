#include "test.h"
#include "../Log/Logger.h"
#include <event.h>
#include <openssl/opensslconf.h>
#include <iostream>


Demo2::Demo2(){  
    std::cout << "create Demo2!" << std::endl;
}

Demo2::~Demo2(){
    std::cout << "destory Demo2!" << std::endl;
}

