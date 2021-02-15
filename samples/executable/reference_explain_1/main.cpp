#include <iostream>
#include <chrono>
#include <cstdint>
#include <iostream>

void passRef( const int& valRef ) {
    printf("%d", valRef);
}

void passVal( int val ) {
    printf("%d", val);
}

class Counter
{
private:
    uint64_t    _counter;
public:
    Counter()
        : _counter(0)
    {}
    void reset() {
        _counter = 0;
    }
    void enref(const uint32_t &val){
        _counter += val;
        enref2(val);
    }
    void enref2(const uint32_t& val) {
        _counter += val;
    }
    void enval(uint32_t val ) {
        _counter += val;
        enval2(val);
    }
    void enval2(uint32_t val) {
        _counter += val;
    }
    operator uint64_t() const {
        return _counter;
    }
};

int main() {
    constexpr size_t numberCount = 1024*1024*128;
    Counter counter;
    uint32_t* numbers = new uint32_t[numberCount];
    for( uint32_t i = 0; i<numberCount; ++i ) {
        numbers[i] = i;
    }
    using TimePoint = decltype(std::chrono::steady_clock::now());
    {
        int64_t cost = 0;
        TimePoint start = std::chrono::steady_clock::now();
        for( size_t i = 0; i<numberCount; ++i ) {
            counter.enref(numbers[i]);
        }
        TimePoint finished = std::chrono::steady_clock::now();
        cost = (finished - start).count();
        std::cout<<"cost - ref :"<<cost<<std::endl;
        std::cout<<counter<<std::endl;
    }
    counter.reset();
    {
        int64_t cost = 0;
        TimePoint start = std::chrono::steady_clock::now();
        for( size_t i = 0; i<numberCount; ++i ) {
            counter.enval(numbers[i]);
        }
        TimePoint finished = std::chrono::steady_clock::now();
        cost = (finished - start).count();
        std::cout<<"cost - val :"<<cost<<std::endl;
        std::cout<<counter<<std::endl;
    }

    return 0;
}