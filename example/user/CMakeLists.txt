# set(SRC_LIST userservice.cc ../user.pb.cc)
set(SRC_LIST server.cc user.pb.cc)

add_executable(user_server ${SRC_LIST})
target_link_libraries(user_server mprpc)