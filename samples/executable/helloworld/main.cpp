#include <cstdint>
#include <atomic>
#include <thread>
#include <cassert>

// int __cdecl hook_func() {
// }


int __stdcall original_func( int a, float b) {
    return a + b;
}

int __cdecl hook_func_real(int a, float b) {
    original_func(a, b);
}

using OriginFunc = decltype(original_func);

int main() {
    ((OriginFunc*)hook_func_real)(1, 2.0f);
	return 0;
}
