#!/usr/bin/bash

if [ $# -lt 1 ]; then
    echo "parameter not specified"
else
    #build and run demo
    if [ $1 = "-b" ] || [ $1 = "-B" ]; then 
        echo "start building all demo"
        cd build && make && cd ..
        if [ -e build/demo$2/demo$2 ]; then
            build/demo$2/./demo$2
        else
            echo "demo$2 does not exist"
        fi
    #mkdir and create file for a not exist demo
    elif [ $1 = "-m" ] || [ $1 = "-M" ]; then
        if [ ! -z "$2" ]; then
            echo "start making dir and file for demo$2"
            if [ -d demo$2 ]; then
                echo "demo$2 already exists"
            else
                mkdir demo$2 && cd demo$2 && touch CMakeLists.txt demo$2.cpp demo$2.h test.cpp && cd ..
                if [ -e demo$2/CMakeLists.txt ] &&
                    [ -e demo$2/demo$2.cpp ] &&
                    [ -e demo$2/demo$2.h ] &&
                    [ -e demo$2/test.cpp ]; then
                    echo -e "add_executable(demo$2 demo$2.cpp test.cpp)" > demo$2/CMakeLists.txt
                    echo -e "target_link_libraries(demo$2 event libevent::core)" >> demo$2/CMakeLists.txt
                    echo -e "#pragma once" > demo$2/demo$2.h
                    echo "make demo$2 file successfuly"
                else
                    echo "make demo$2 file failed"
                fi
            fi
        else
            echo "please add digital parameter after "-m" or "-M" to specify the number of demo."
        fi
    # TO DO
    # Can add others parameter for others function.
    # invaild parameters
    else 
        echo "'$1' is not a vaild parameter, please refer to help."
    fi
fi
