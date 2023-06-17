#include <fstream>
#include <iostream>
#include <filesystem>
#include "mprpcconfig.h"
// 解析文件
void MprpcConfig::LoadConfigFile(const char* config_file) {
    namespace fs = std::filesystem;
    std::cout << fs::path(config_file).string() << "\n";
    fs::path file_path(config_file);
    if (fs::exists(config_file)) {

    std::ifstream file(config_file);

    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        if (line.size() && line[0] == '#') continue;
        auto p = line.find("=");
        if (p != -1) {
            auto key = line.substr(0, p);
            int pos = key.find_first_not_of(' ');
            if (pos != -1) {
                key = key.substr(pos);
            }
            auto value = line.substr(p + 1);
            mp_[key] = value;
            std::cout << key << ":" << value << "\n";
        }
      }
      file.close();
    } else {
      std::cerr << "Error: Unable to open file." << std::endl;
    }
  } else {
    std::cerr << "Error: File does not exist." << std::endl;
  }
}

std::string MprpcConfig::Load(const std::string& key) {
    auto it = mp_.find(key);
    if (it != mp_.end()) {
        return it->second;
    }
    return "";
}