#include <cstdio>
#include <cstdint>

// 函数示例
uint32_t factorial( uint32_t _value ) {
    uint32_t v = _value;
    while(--_value) {
        v *= _value;
    }
    return v;
}
//

int main() {
    uint32_t value = 3;
    uint32_t v = factorial( value );
    printf("factorial of %u is %u\n", value, v);
    return 0;
}