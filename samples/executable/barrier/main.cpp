#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>

class Barrier {
private:
    std::mutex                              _mutex;
    std::condition_variable                 _conditionVariable;
    uint32_t                                _criticalCount;
    std::atomic<uint32_t>                   _counter;
public:
    Barrier( uint32_t criticalCount )
        : _mutex()
        , _conditionVariable()
        , _criticalCount(criticalCount)
        , _counter(0)
    {}

    void wait() {
        ++_counter;
        uint32_t criticalCountCopy =  _criticalCount;
        bool rst = _counter.compare_exchange_strong(criticalCountCopy, 0, std::memory_order::memory_order_relaxed );
        std::unique_lock<std::mutex> lock(_mutex);
        if(!rst) {
            _conditionVariable.wait(lock);
        }
        else {
            _conditionVariable.notify_all();
        }
    }
};

uint32_t counters[4];

void threadFunc( uint32_t* counter, Barrier* barrier ) {
    for( uint32_t i = 0; i<10000; ++i) {
        (*counter)+=i;
    }
    barrier->wait();
}

int main() {
    Barrier barrier(5);
    std::thread threads[4];
    for(uint32_t threadIndex = 0; threadIndex<4; ++threadIndex) {
        threads[threadIndex] = std::thread( threadFunc, &counters[threadIndex], &barrier);
    }
    barrier.wait();
    uint32_t totalCount = 0;
    for( uint32_t idx = 0; idx<4; ++idx) {
        totalCount += counters[idx];
    }
    return totalCount;
}