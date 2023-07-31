cmake_minimum_required(VERSION 3.20)

project(libevent-test)
find_package(Libevent REQUIRED)

add_subdirectory(demo1)

add_subdirectory(demo2)