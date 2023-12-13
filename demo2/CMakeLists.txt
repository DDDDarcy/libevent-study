add_executable(demo2 demo.cpp test.cpp demo2.cpp)

set(CMAKE_CXX_FLAGS " -g ")

target_link_libraries(demo2 event libevent::core pthread)