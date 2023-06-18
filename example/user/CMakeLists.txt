# set(SRC_LIST userservice.cc ../user.pb.cc)
set(server server.cc user.pb.cc)

add_executable(user_server ${server})
target_link_libraries(user_server mprpc)



set(client client.cc user.pb.cc)

add_executable(user_client ${client})
target_link_libraries(user_client mprpc)