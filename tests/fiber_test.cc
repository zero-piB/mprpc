#include <thread>
#include <vector>
#include "fiber.h"

void run_in_fiber_fn() {
    LOG_INFO << "sub fiber start, fiber_id = " << Fiber::GetFiberId();
    Fiber::YieldToHold();
    LOG_INFO << "sub fiber end, fiber_id = " << Fiber::GetFiberId();
    Fiber::YieldToHold();
}


void test_fiber(int t_pid) {
    {
        auto main_fiber_ptr = Fiber::GetThis();
        LOG_INFO << "main fiber start, thread_id = " << t_pid;
        Fiber::ptr sub_fiber(new Fiber(&run_in_fiber_fn));
        sub_fiber->swapIn();
        LOG_INFO << "main fiber after swap in, thread_id = " << t_pid;
        sub_fiber->swapIn();
        LOG_INFO << "main fiber end, thread_id = " << t_pid;
        sub_fiber->swapIn();
    }
}

int main() {
    using namespace std;
    int n = 5;
    vector<shared_ptr<thread>> ths;
    ths.resize(n);
    for (int i = 0; i < n; i++) {
        ths[i] = make_shared<thread>(&test_fiber, i);
    }
    for (int i = 0; i < n; i++) {
        ths[i]->join();
    }
    return 0;
}