
#include "mprpcchannel.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void MprpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done) 
{

    std::string server_name = method->service()->name();
    std::string method_name = method->name();
    
    std::string args;

    if (!request->SerializeToString(&args)) {
        std::cout << "反序列化失败" << "\n";
        return;
    } 

    mprpc::RpcHeader rpc_header;
    rpc_header.set_server_name(server_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args.size());

    std::string header_str;
    if (!rpc_header.SerializeToString(&header_str)) {
        std::cout << "序列化头部失败\n";
    }

    std::string rpc_string;
    uint32_t header_size = header_str.size();
    rpc_string.append(std::string(reinterpret_cast<char*>(&header_size), 4));
    rpc_string.append(header_str);
    rpc_string.append(args);


    std::cout << " =========================== \n";
    std::cout << "header str: " << header_str << " size: " << header_str.size() << "\n";

    std::cout << "server_name: " << server_name << "\n";
    std::cout << "method_name: " << method_name << "\n";
    std::cout << "args: " << args << " size=" << args.size() << "\n";

    std::cout << " =========================== \n";

    // 使用tcp，完成远程调用
    // 创建 socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return ;
    }
     // 设置服务器地址和端口号
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_.c_str()); // 服务器 IP 地址
    server_addr.sin_port = htons(port_); // 服务器端口号

     // 连接服务器
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    if (send(sock, rpc_string.c_str(), rpc_string.length(), 0) == -1) {
        std::cerr << "Failed to send message." << std::endl;
    }

    char rsp[400];
    if (read(sock, rsp, 400) == -1) {
        std::cerr << "Failed to send message." << std::endl;
    }
    // 关闭 socket
    close(sock);

    response->ParseFromString(rsp);
}