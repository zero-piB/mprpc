#include <string>
#include "user.pb.h"

class UserServerImpl {

public:
    bool Login(const std::string& name, const std::string& pwd) {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl; 
        return true;
    }

    void Login(::google::protobuf::RpcController* controller,
                       const ::mprpc::LoginRequest* request,
                       ::mprpc::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd); 

        // 把响应写入  包括错误码、错误消息、返回值
        mprpc::UserRespone *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_sucess(login_result);

        // 执行回调操作   执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
}

int main() {
    
    return 0;
}