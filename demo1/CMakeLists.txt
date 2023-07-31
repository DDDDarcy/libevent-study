add_executable(demo1 demo.cpp test.cpp)

#find_package(Libevent REQUIRED)

target_link_libraries(demo1 event libevent::core)