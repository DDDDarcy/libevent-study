#include "test.h"
#include "../Log/Logger.h"
#include <event.h>
#include <openssl/opensslconf.h>
#include <iostream>


Demo2::Demo2(){  
    LOG(LogLevel::DEBUG, "create Demo2!~");
}

Demo2::~Demo2(){
    LOG(LogLevel::DEBUG, "destory Demo2!~");
}

