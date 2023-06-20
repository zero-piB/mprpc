#include <string>
#include "macro.h"

void foo() {
    // std::cout << BacktraceToString(5, 2);
    // std::cout << rollx::get_backtrace();
    ROLLX_ASSERT(false);
}
void Bar() {
    foo();
}
int main() {
    Bar();
    return 0;
}