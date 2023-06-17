#include <iostream>
#include <unistd.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "mprpcapplication.h"

// 初始化的参数
ABSL_FLAG(std::string, port, "50051", "Server port for the service");

void MprpcApplication::Init(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    std::cout << absl::GetFlag(FLAGS_port) << "\n";
}

MprpcApplication& MprpcApplication::getInstance() {
    static MprpcApplication self;
    return self;
}