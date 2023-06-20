cmake_minimum_required(VERSION 3.0)

project(mprpc)

# 可以调试的版本
set(CMAKE_BUILD_TYPE "Debug")
# 设置编译选项
add_compile_options(-Wall)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/src/include)
include_directories(${CMAKE_SOURCE_DIR}/src/common/include)
include_directories(${CMAKE_SOURCE_DIR}/example/user)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/example)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

