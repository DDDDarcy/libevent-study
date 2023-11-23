cmake_minimum_required(VERSION 3.20)

project(libevent-test)
find_package(Libevent REQUIRED)
find_package(Protobuf REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(PROTOBUF_LIBRARIES /usr/local/lib)
set(PROTOBUF_CFLAGS )
if(PROTOBUF_FOUND)
    message(STATUS "Found Protocol Buffers: ${PROTOBUF_LIBRARIES}")
    message(STATUS "cflags :${PROTOBUF_CFLAGS} " )
else()
    message(FATAL_ERROR "Protocol Buffers not found.")
endif()



add_subdirectory(demo1)

add_subdirectory(demo2)

add_subdirectory(demo3)

add_subdirectory(demo4)

