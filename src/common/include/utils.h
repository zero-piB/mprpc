#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <regex>
#include <dlfcn.h>


namespace rollx {

static std::string get_backtrace() {
    const int max_frames = 64;
    void* frames[max_frames];
    int num_frames = backtrace(frames, max_frames);
    char** symbols = backtrace_symbols(frames, num_frames);
    std::string backtrace_str;
    char stack_info[256];
    for (int i = 0; i < num_frames; i++) {
        Dl_info info;
        if (dladdr(frames[i], &info) && info.dli_sname) {
            int status = 0;
            char* demangled_name = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
            if (status == 0) {
                snprintf(stack_info, sizeof(stack_info), "%d: %s (%s:%p)\n", i, demangled_name, info.dli_fname, info.dli_saddr);
                free(demangled_name);
            } else {
                snprintf(stack_info, sizeof(stack_info), "%d: %s (%s:%p)\n", i, info.dli_sname, info.dli_fname, info.dli_saddr);
            }
        } else {
            snprintf(stack_info, sizeof(stack_info), "%d: %s\n", i, symbols[i]);
        }
        backtrace_str.append(stack_info);
    }
    free(symbols);
    return backtrace_str;
}

} //rollx
