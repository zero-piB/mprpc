
# mprpc

基于module实现的网路库 + protobuf实现的数据编码协议 实现的rpc框架

使用方式可看example


# protobuf 

## 安装
安装protobuf, C++版本需要c++14以上, 需要预先安装cmake和gcc

安装protobuf

``` shell
git cloen git@github.com:protocolbuffers/protobuf.git
cd protobuf

git submodule update --init --recursive 

mkdir -p build && cd build

cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_CXX_STANDARD=14 ..
make -j32 && make install

```

## 使用

加上 option cc_generic_services = true; 会生成服务类

