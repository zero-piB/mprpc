#include <iostream>
#include <unistd.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "mprpcapplication.h"

void MprpcApplication::Init(int argc, char** argv) {
    // absl::ParseCommandLine(argc, argv);
}

MprpcApplication& MprpcApplication::getInstance() {
    static MprpcApplication self;
    return self;
}