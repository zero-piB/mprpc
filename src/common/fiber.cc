/*
 * @Author: yangwendeng
 * @Date: 2023-06-20 10:03:10
 * @FilePath: /mprpc/src/common/fiber.cc
 */
#include "fiber.h"
#include "macro.h"
#include <atomic>

static std::atomic<uint64_t> s_fiber_id;    // 当前协程id
static std::atomic<uint64_t> s_fiber_count; // 运行的协程数量

static thread_local Fiber* t_fiber = nullptr;   // 子协程
static thread_local Fiber::ptr t_threadFiber = nullptr; // 线程内的主协程

// 128K 
const static uint32_t g_fiber_stack_size = 128 * 1024;

// 分配内存
class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }

    static void Dealloc(void* vp, size_t size) {
        return free(vp);
    }
};

using StackAllocator = MallocStackAllocator;

// 线程第一个协程的构造, 只允许内部创建
Fiber::Fiber() {
    state_ = EXEC;
    SetThis(this);
    
    if (getcontext(&ctx_)) {
        ROLLX_ASSERT(false);
    }
    ++s_fiber_count;
    LOG_DEBUG << "fiber main";
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller):
    id_(++s_fiber_id),
    cb_(cb) 
{
    ++s_fiber_count;
    stack_size_ = stacksize > 0 ? stack_size_: g_fiber_stack_size;
    stack_ptr_ = StackAllocator::Alloc(stack_size_);

    if (getcontext(&ctx_)) {
        ROLLX_ASSERT(false);
    }
    // 为当前上下问分配执行栈
    ctx_.uc_link = nullptr;
    ctx_.uc_stack.ss_sp = stack_ptr_;
    ctx_.uc_stack.ss_size = stack_size_;

    if (!use_caller) {
        makecontext(&ctx_, &Fiber::MainFunc, 0);
    } else {
        makecontext(&ctx_, &Fiber::CallerMainFunc, 0);
    }

    LOG_DEBUG << "Fiber created, id = " << id_;
}

Fiber::~Fiber() {
    --s_fiber_count;
    if (stack_ptr_) {
        // 如果是子协程
        ROLLX_ASSERT(state_ == INIT || state_ == EXCEPT || state_ == TERM);
        StackAllocator::Dealloc(stack_ptr_, stack_size_);
    } else {
        // 如果是主协程
        ROLLX_ASSERT(!cb_);
        ROLLX_ASSERT(state_ == EXEC);

        Fiber* cur = t_fiber;
        if(cur == this) {
            SetThis(nullptr);
        }
    }

    LOG_DEBUG << "Fiber::~Fiber id=" << id_
                              << " total=" << s_fiber_count;
}

//重置协程函数，并重置状态
//INIT，TERM, EXCEPT
void Fiber::reset(std::function<void()> cb) {
    ROLLX_ASSERT(stack_ptr_);
    ROLLX_ASSERT(state_ == TERM
            || state_ == EXCEPT
            || state_ == INIT);
    cb_ = cb;
    
    if(getcontext(&ctx_)) {
        ROLLX_ASSERT2(false, "getcontext");
    }
     // 重新分配ctx
    ctx_.uc_link = nullptr;
    ctx_.uc_stack.ss_sp = stack_ptr_;
    ctx_.uc_stack.ss_size = stack_size_;
    makecontext(&ctx_, &Fiber::MainFunc, 0);
    state_ = INIT;
}

// 子协程换入，主协程换出
void Fiber::swapIn() {
    SetThis(this);
    ROLLX_ASSERT(state_ != EXEC);
    state_ = EXEC;
    if (swapcontext(&t_threadFiber->ctx_, &ctx_)) {
        ROLLX_ASSERT2(false, "swapcontext");
    }
}

// 子协程换出，主协程换入
void Fiber::swapOut() {
    SetThis(t_threadFiber.get());
    if (swapcontext(&ctx_, &t_threadFiber->ctx_)) {
        ROLLX_ASSERT2(false, "swapcontext");
    }
}

//设置当前协程
void Fiber::SetThis(Fiber* f) {
    t_fiber = f;
}

// 返回当前协程
Fiber::ptr Fiber::GetThis() {
    if (t_fiber) {
        return t_fiber->shared_from_this();
    }

    Fiber::ptr main_fiber(new Fiber);
    // 创建主协程后，肯定调用了setThis
    ROLLX_ASSERT(t_fiber == main_fiber.get());
    t_threadFiber = main_fiber;
    return main_fiber->shared_from_this();
}
