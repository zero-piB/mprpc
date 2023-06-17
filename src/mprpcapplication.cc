#include <iostream>
#include <unistd.h>
#include <filesystem>

#include "mprpcapplication.h"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

MprpcConfig MprpcApplication::configer_;
namespace fs = std::filesystem;

// 初始化的参数
std::string default_config_file = fs::path(std::string(__FILE__)).parent_path().string() + "/../config.conf";
ABSL_FLAG(std::string, config_file, default_config_file, "配置文件");

void MprpcApplication::Init(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    auto config_file = absl::GetFlag(FLAGS_config_file) ;
    configer_.LoadConfigFile(config_file.c_str());
}

MprpcApplication& MprpcApplication::getInstance() {
    static MprpcApplication self;
    return self;
}