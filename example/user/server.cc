#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class UserServerImpl: public mprpc::UserServer {

public:
    bool Login(const std::string& name, const std::string& pwd) {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl; 
        if (name == "ywd" && pwd == "Ling0000") return true;
        return false;
    }

    void Login(::google::protobuf::RpcController* controller,
                       const ::mprpc::LoginRequest* request,
                       ::mprpc::LoginRespone* response,
                       ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->username();
        std::string pwd = request->passwd();

        // 做本地业务
        bool login_result = Login(name, pwd); 

        if (login_result) {
            response->set_status(0);
            response->set_msg("welcome, ywd");
        } else {
            response->set_status(-1);
            response->set_msg("user not exist");
        }
        
        // 执行回调操作   执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
};

int main(int argc, char** argv) {
    // 初始化
    MprpcApplication::getInstance().Init(argc, argv);
    RpcProvider provider;
    // 注册
    provider.NotifyServer(new UserServerImpl());
    // 启动
    provider.Run();
    return 0;
}