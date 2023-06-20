
# mprpc

基于module实现的网路库 + protobuf实现的数据编码协议 实现的rpc框架

使用方式可看example


# protobuf 

## 安装
安装protobuf, C++版本需要c++14以上, 需要预先安装cmake和gcc

安装gcc
yum groupinstall "Development Tools"
yum install gmp-devel mpfr-devel libmpc-devel
cd gcc-xx
./configure --disable-multilib --enable-bootstrap --enable-languages=c,c++

上面的命令会把gcc自动安装到 /usr/local下面, 加上 --prefix=xxxx选定指定的安装路径

安装cmake



安装protobuf

``` shell
git clone git@github.com:protocolbuffers/protobuf.git
cd protobuf

git submodule update --init --recursive 

mkdir -p build && cd build

cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_CXX_STANDARD=14 ..
make -j32 && make install

此外还要安装muduo网络库，这个库依赖了下面的包(centos7)

yum install gcc-c++ cmake make boost-devel

貌似对boost库的版本会有一定的要求，不然会编译不过

安装教程: https://blog.csdn.net/this_capslock/article/details/47170313

```
**记得以后编译其他库时，都加上 set(CXX_FLAGS -fPIC)**

## 使用

加上 option cc_generic_services = true; 会生成服务类

