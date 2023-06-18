#pragma once

#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

#include "rpcheader.pb.h"

using namespace google::protobuf;

class MprpcChannel: public google::protobuf::RpcChannel {

public:
    explicit MprpcChannel(const std::string& endpoint): endpoint_(endpoint) {
        auto p = endpoint.find(":");
        if (p != -1) {
            ip_ = endpoint.substr(0, p);
            port_ = std::stoi(endpoint.substr(p + 1));
        }
    }
private:
    virtual void CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done);
    std::string endpoint_;
    std::string ip_;
    uint16_t port_;
};

