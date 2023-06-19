#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

template<typename T>
class ConcurentQueue {

public:
    void Push(const T& val) {
        std::unique_lock<std::mutex> lk(mu);
        q.push(val);
        lk.unlock();
        cv.notify_one();
    }
    T WaitAndPop() {
        std::unique_lock<std::mutex> lk(mu);
        cv.wait(lk, [&](){ return !q.empty(); });
        T ret = q.front();
        q.pop();
        return ret;
    }
    bool Empty() {
        std::scoped_lock<std::mutex> lk(mu);
        return q.empty();
    }
    size_t Size() {
        std::scoped_lock<std::mutex> lk(mu);
        return q.size();
    }
private:
    std::queue<T> q;
    std::condition_variable cv;
    std::mutex mu;
};