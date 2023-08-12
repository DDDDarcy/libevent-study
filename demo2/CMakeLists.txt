add_executable(demo2 demo.cpp test.cpp)

target_link_libraries(demo2 event libevent::core)