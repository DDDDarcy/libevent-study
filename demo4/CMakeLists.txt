add_executable(demo4 
                singleton.cpp
                demo4.cpp
                server.cpp
                client.cpp
                stateMechine.cpp 
                test.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
target_link_libraries(demo4 event libevent::core pthread)
