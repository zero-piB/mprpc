
aux_source_directory(. SRC_LIST)

find_package(protobuf REQUIRED)
find_package(absl REQUIRED)
# 添加可执行文件
add_library(mprpc SHARED ${SRC_LIST})

target_link_libraries(mprpc
    protobuf::libprotobuf
    absl::base absl::strings absl::flags_parse
)