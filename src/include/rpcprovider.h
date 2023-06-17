#pragma once

#include "google/protobuf/service.h"

class RpcProvider {
public:
    // 注册服务
    void NotifyServer(google::protobuf::Service* server);
    // 启动rpc服务
    void Run();
};