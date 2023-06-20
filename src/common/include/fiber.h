/*
 * @Author: yangwendeng
 * @Date: 2023-06-20 10:02:54
 * @FilePath: /mprpc/src/common/include/fiber.h
 */
#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <functional>
#include <ucontext.h>

class Fiber: public std::enable_shared_from_this<Fiber> {
public:
    using ptr = std::shared_ptr<Fiber>;
    /**
     * @brief 协程状态
     */
    enum State {
        /// 初始化状态
        INIT,
        /// 暂停状态
        HOLD,
        /// 执行中状态
        EXEC,
        /// 结束状态
        TERM,
        /// 可执行状态
        READY,
        /// 异常状态
        EXCEPT
    };
private:
     /**
     * @brief 无参构造函数
     * @attention 每个线程第一个协程的构造
     */
    Fiber();
public:
    /**
     * @brief 构造函数
     * @param[in] cb 协程执行的函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否在MainFiber上调度
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);
    
    /**
     * @description: 析构函数
     */
    ~Fiber();

    /**
     * @description: 重置协程的执行函数，并设置状态
     * @pre getState() 为 INIT, TERM, EXCEPT
     * @post getState() = INIT
     * @param {function<void()>} cb 执行函数
     */    
    void reset(std::function<void()> cb);

    /**
     * @description: 将当前西城切换到运行状态
     * @pre getState() != EXEC
     * @post getState() = EXEC
     */
    void swapIn();

    /**
     * @description: 将当前协程切换到后台
     */
    void swapOut();

    /**
     * @description: 返回协程id
     * @return 协程id
     */    
    uint64_t getId() const { return id_; }

    /**
     * @description: 返回当前协程状态
     * @return 协程状态State
     */    
    State getState() const { return state_; }

public:
    /**
     * @description: 设置当前线程的运行协程
     * @param {Fiber*} f
     */    
    static void SetThis(Fiber* f);

    /**
     * @description: 返回当前所运行的协程
     * @return Fiber::ptr (std::shared_ptr<Fiber>)
     */    
    static Fiber::ptr GetThis();

    /**
     * @description: 将当前的协程切换到后台，并设置为ready状态
     */
    static void YieldToReady();

    /**
     * @description: 将当前的协程切换到后台，并设置为hold状态
     */
    static void YieldToHold();

    /**
     * @description: 返回当前协程的总数量
     * @return 当前协程的总数量
     */    
    static uint64_t TotalFibers();

    /**
     * @description: 协程执行函数
     * @post 执行完成返回到线程的 主协程内
     */
    static void MainFunc();

    /**
     * @description: 协程执行函数
     * @post 执行完成返回到线程调度协程
     */
    static void CallerMainFunc();

    /**
     * @description: 返回当前协程的Id
     * @return 当前协程的Id
     */
    static uint64_t GetFiberId();

private:
    uint64_t id_ = 0;   // 协程id
    State state_ = INIT;   // 协程状态
    uint32_t stack_size_ = 0;   // 协程栈大小
    ucontext ctx_;     // 当前协程上下文
    void* stack_ptr_ = nullptr;     // 当前协程的运行栈指针
    std::function<void()> cb_;      // 当前协程的运行函数
};
