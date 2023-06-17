#include <functional>
#include "google/protobuf/stubs/callback.h"
#include "rpcprovider.h"
#include "rpcheader.pb.h"

// 实现服务，方法的描述
void RpcProvider::NotifyServer(google::protobuf::Service* server) {
    ServerInfo* server_info = new ServerInfo();
    // 服务描述
    const ::google::protobuf::ServiceDescriptor* srv_descriptor = server->GetDescriptor();
    server_info->server = server;
    int method_cnt = srv_descriptor->method_count();

    for (int i = 0; i < method_cnt; i++) {
        const google::protobuf::MethodDescriptor* method_desc = srv_descriptor->method(i);
        server_info->methods.insert({method_desc->name(), method_desc});
    }
    server_infos_.insert({srv_descriptor->name(), server_info});
}

void RpcProvider::Run() {
    std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserverip");
    std::string port = MprpcApplication::getInstance().getConfig().Load("rpcserverport");
    muduo::net::InetAddress ipaddr(ip.c_str(), std::stoi(port));
    // 创建
    muduo::net::TcpServer server(&eventLoop_, ipaddr, "RpcProvider");

    // 绑定连接回调和消息读写回调方法， 分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, 
        std::placeholders::_2, std::placeholders::_3));

    // 线程库的数量
    server.setThreadNum(4);
    // 启动服务
    server.start();
    std::cout << "server run on " << ip << ":" << port << "\n";
    eventLoop_.loop();
}

// 连接时的事件
void RpcProvider::OnConnection(const TcpConnectionPtr& conn) {
    if (!conn->connected()) {
        conn->shutdown();
    }
}

// 与已连接的读写事件
/*
分发服务，servername，methodname，args来进行分发到上层对应的服务, 但是在protobuf里面都是一串二进制
所以需要约定一个存储的格式，分别是 header_size + header_str + args_str
*/
void RpcProvider::OnMessage(const TcpConnectionPtr& conn, Buffer* buffer, muduo::Timestamp) {
    std::string rev_buf = buffer->retrieveAllAsString();
    // 读取前4个字节的内容
    uint32_t header_size;
    rev_buf.copy((char*)&header_size, 4, 0);
    // 根据header_size读取原始的字符流, 反序列化得到原始信息
    std::string rpc_header_str = rev_buf.substr(4, header_size);
    mprpc::RpcHeader rpc_header;
    if (rpc_header.ParseFromString(rpc_header_str)) {
        std::cout << "Serialize success\n";
    } else {
        std::cout << "Serialize fail\n";
    }

    std::string args_str = rev_buf.substr(4 + header_size);
    auto server_name = rpc_header.server_name();
    auto method_name = rpc_header.method_name();
    uint32_t args_size = rpc_header.args_size();

    std::cout << rpc_header_str << "\n";
    std::cout << "server_name: " << server_name << "\n";
    std::cout << "method_name: " << method_name << "\n";
    std::cout << "args_size: " << args_size << "\n";
    std::cout << "args_str: " << args_str << "\n";

    // 获取server对象和method对象
    auto it = server_infos_.find(server_name);
    if (it == server_infos_.end()) {
        std::cout << " not exist server: " << server_name << "\n";
        return;
    }

    google::protobuf::Service* server = it->second->server;
    auto method_it = it->second->methods.find(method_name);
    if (method_it == it->second->methods.end()) {
        std::cout << "methods not exist : " << method_name << "\n";
        return;
    }

    const google::protobuf::MethodDescriptor* method_descriptor = method_it->second;
    // 生成对应的参数
    google::protobuf::Message* request = server->GetRequestPrototype(method_descriptor).New();
    if (request->ParseFromString(args_str)) {
        std::cout << " ParseFromString successed\n";
    } else {
        std::cout << " ParseFromString fail\n";
        return;
    }
    // 结果
    google::protobuf::Message* response = server->GetResponsePrototype(method_descriptor).New();

    // 自己会生成一个Closure类
    google::protobuf::Closure* done = 
        google::protobuf::NewCallback<RpcProvider,const TcpConnectionPtr&,google::protobuf::Message*>
                                (this, 
                                &RpcProvider::SendRpcResponse, 
                                conn, 
                                response);
    
    // 调用业务方法
    server->CallMethod(method_descriptor, nullptr, request, response, done);
}

// 反序列化，并发送到网络上
void RpcProvider::SendRpcResponse(const TcpConnectionPtr& conn, google::protobuf::Message* response) {
    std::string msg;
    if (response->SerializeToString(&msg)) {
        conn->send(msg);
        // 短连接，断开
        conn->shutdown();
    } else {
        std::cout << "反序列化失败" << "\n";
    }
}