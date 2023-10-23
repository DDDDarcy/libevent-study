
set(SRC socket.cpp msg.pb.cc)
add_executable(demo3 demo3.cpp test.cpp ${SRC})


target_link_libraries(demo3 event libevent::core protobuf)
