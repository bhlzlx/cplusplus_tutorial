#include <cstdio>
#include <cstdint>
#include <memory.h>

const char* textSample1 = "hello,world!";
const char textSample2[] = "hello,world!";
const int VVVV = 0xBEAFBEAF;

struct StructSample {
    uint32_t    uValue;
    float       fValue;
    char        carr[4];
    short       sarr[2];
};
constexpr size_t StructSize = sizeof(StructSample);

union UnionSample {
    StructSample    s;
    uint32_t        uarr[4];
};

uint32_t sampleSum( UnionSample _un ) {
    UnionSample u = _un;
    uint32_t s = u.uarr[0] + u.uarr[1] + u.uarr[2] + u.uarr[3];
    return s;
}

void funcAAA( uint32_t _value ) {
    uint32_t v = _value;
    printf("%p\n", &v);
}

void funcBBB( float _value ) {
    float s = _value;
    printf("%p\n", &s);
    funcBBB(s);
}

int main( int _argc, char** _argv ) {

    int arr[10240000];

    printf("%s", textSample1);
    //
    StructSample sSample;   // sizeof( StructSample ) == 16
    uint32_t arrSample[4];  // sizeoof(uint32_t) * 4 == 16
    UnionSample uSample;    // 同样也是16字节大小
    UnionSample* samplePtr = new UnionSample(); // 同样效果的方法还有 malloc,calloc 等
    //
    memcpy(samplePtr, &sSample, sizeof(StructSample));
    uint32_t sum = sampleSum(*samplePtr );
    //
    printf("%u\n", sum);
    //
    printf(
        " %p\n %p\n %p\n %p\n %p\n", 
        &sSample,
        &arrSample,
        &uSample,
        &samplePtr,
        &sum
    );

    // funcAAA(64);
    // funcBBB(3.1415926f);

    { // funcAAA
        uint32_t v = 64;
        printf("%p\n", &v);
    }
    { // func bbb 
        float s = 3.1415926f;
        printf("%p\n", &s);
    }
    // printf("%u", v);

    return 0;
}