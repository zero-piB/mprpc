#pragma once

#include <string.h>
#include <assert.h>
#include "simplelogger.h"
#include "utils.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define LIKELY(x)      (x)
#   define UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define ROLLX_ASSERT(x) \
    if(UNLIKELY(!(x))) { \
        LOG_ERROR << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << rollx::get_backtrace(); \
        assert(x); \
    }

/// 断言宏封装
#define ROLLX_ASSERT2(x, w) \
    if(UNLIKELY(!(x))) { \
        LOG_ERROR << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << rollx::get_backtrace(); \
        assert(x); \
    }

