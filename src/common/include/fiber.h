#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <ucontext.h>

class Fiber: public std::enable_shared_from_this<Fiber> {

public:
    std::shared_ptr<Fiber> get_ptr() {
        return shared_from_this();
    }
};
