#pragma once

#include <memory>
#include <unordered_map>
#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include "mprpcapplication.h"

class RpcProvider {

struct ServerInfo {
    google::protobuf::Service* server;
    std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methods;
};

using TcpConnectionPtr = std::shared_ptr<muduo::net::TcpConnection>;
using Buffer = muduo::net::Buffer;

public:
    // 注册服务
    void NotifyServer(google::protobuf::Service* server);
    // 启动rpc服务
    void Run();
    
private:
    muduo::net::EventLoop eventLoop_;   

    // 新socket建立的回调函数
    void OnConnection(const TcpConnectionPtr& conn);

    void OnMessage(const TcpConnectionPtr&, Buffer*, muduo::Timestamp);

    // Done() 方法调用, 序列化和发送请求
    void SendRpcResponse(const TcpConnectionPtr&, google::protobuf::Message*);

    std::unordered_map<std::string, ServerInfo*> server_infos_;
};