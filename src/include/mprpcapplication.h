#pragma once

#include "mprpcconfig.h"

class MprpcApplication {
public:
    static void Init(int argc, char** argv);
    static MprpcApplication& getInstance();
    static MprpcConfig& getConfig();
private:
    MprpcApplication() {};
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
    static MprpcConfig configer_;
};