#include <iostream>

#include "user.pb.h"
#include "mprpcchannel.h"
#include "mprpcapplication.h"

int main(int argc, char **argv) {
    MprpcApplication::Init(argc, argv);

    MprpcChannel channel("0.0.0.0:8000");
    mprpc::UserServer_Stub stub_(&channel);

    mprpc::LoginRequest request;

    mprpc::LoginRespone response;
    google::protobuf::Closure *done = nullptr;
    
    request.set_username("ywd");
    request.set_passwd("Ling0000");
    // 同步的调用过程
    stub_.Login(nullptr, &request, &response, done);

    if (response.status() == 0) {
        std::cout << "登陆成功\n" << response.msg() << "\n";
    } else {
        std::cout << "登陆失败\n" << response.msg() << "\n";
    }

    return 0;
}