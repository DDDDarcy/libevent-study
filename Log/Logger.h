#pragma once
#include <iostream>


#define LOG(LogLevel, output)                                       \
    do{                                                             \
        switch(LogLevel)                                            \
        {                                                           \
            case DEBUG: std::cout << "Debug: "                      \
                        << output << "." << std::endl;              \
                break;                                              \
            case ERROR: std::cout << "Error: "                      \
                        << output << "." << std::endl;              \
                break;                                              \
        }                                                           \
    }while(0)                                                       \

enum LogLevel{
    DEBUG,
    ERROR
};