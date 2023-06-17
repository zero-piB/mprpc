#include "rpcprovider.h"
#include <functional>

void RpcProvider::NotifyServer(google::protobuf::Service* server) {

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

}

// 与已连接的读写事件
void RpcProvider::OnMessage(const TcpConnectionPtr&, Buffer*, muduo::Timestamp) {

}