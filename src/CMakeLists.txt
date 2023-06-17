
# 生成makefile之后，就不会再更新
aux_source_directory(. SRC_LIST)

find_package(protobuf REQUIRED)
find_package(absl REQUIRED)
# 添加可执行文件
add_library(mprpc ${SRC_LIST})

target_link_libraries(mprpc
    protobuf::libprotobuf
    muduo_net muduo_base
    absl::strings absl::flags_parse
)