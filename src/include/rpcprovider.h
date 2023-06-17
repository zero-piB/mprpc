#pragma once

#include <memory>
#include "google/protobuf/service.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include "mprpcapplication.h"

class RpcProvider {

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

    // // std::function<void (const TcpConnectionPtr&,
    //                         Buffer*,
    //                         Timestamp)> MessageCallback;
    void OnMessage(const TcpConnectionPtr&, Buffer*, muduo::Timestamp);
};