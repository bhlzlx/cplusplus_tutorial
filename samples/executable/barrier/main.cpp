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
    uint32_t                                _cycle;
public:
    Barrier( uint32_t criticalCount )
        : _mutex()
        , _conditionVariable()
        , _criticalCount(criticalCount)
        , _counter(0)
    {}

    void wait() {
        ++_counter;
        uint32_t cycle = _cycle;
        uint32_t criticalCountCopy =  _criticalCount;
        bool rst = _counter.compare_exchange_strong(criticalCountCopy, 0, std::memory_order::memory_order_relaxed );
        std::unique_lock<std::mutex> lock(_mutex);
        if(!rst) {
            _conditionVariable.wait(lock, [this,&cycle]()->bool{
                return cycle != _cycle;
            });
        }
        else {
            _conditionVariable.notify_all();
        }
    }
};

class Semaphore {
private:
    std::mutex                  _mutex;
    std::condition_variable     _conditionVariable;
    std::atomic<uint32_t>       _counter;
public:
    Semaphore( uint32_t counter )
        : _mutex()
        , _conditionVariable()
        , _counter(0)
    {}

    void wait() {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_counter == 0) {
            /**
             * @brief 为什么wait(lock, predator ) 后边这个Predator是必要的，
             * 因为Linux上应用会出现中断，在wait的时候中断会唤醒线程。这个时候
             * 并没有notify，所以要加一个predator。
             */
            _conditionVariable.wait(lock, [&]()->bool {
                return _counter != 0;
            }); // 阻塞
        }
        else {
            --_counter; // 不阻塞，正常通过
        }
    }

    void signal() {
        std::unique_lock<std::mutex> lock(_mutex);
        bool needNotify = (0==_counter);
        ++_counter;
        if(needNotify) {
            _conditionVariable.notify_one();
        }
    }
};

class OneWayBlockedBarrier {
private:
    uint32_t                _sigNeed;
    uint32_t                _sigCounter;
    std::mutex              _mutex;
    std::condition_variable _conditionVariable;
public:
    OneWayBlockedBarrier( uint32_t sigNeed )
        : _sigNeed(sigNeed)
        , _sigCounter(0)
        , _mutex()
        , _conditionVariable()
    {}

    void wait() {
        std::unique_lock<std::mutex> lock(_mutex);
        _conditionVariable.wait(lock, [this]()->bool{
            return _sigNeed == _sigCounter;
        });
        _sigCounter = 0; // reset signal counter
    }

    void signal() {
        std::unique_lock<std::mutex> lock(_mutex);
        ++_sigCounter;
        if(_sigCounter == _sigNeed) {
            _conditionVariable.notify_one();
        }
    }
};

uint32_t counters[4];

void threadFuncBarrier( uint32_t* counter, OneWayBlockedBarrier* barrier ) {
    for( uint32_t i = 0; i<10000; ++i) {
        (*counter)+=i;
    }
    barrier->signal();
}

void threadFuncSemaphore( uint32_t* counter, Semaphore* Semaphore ) {

}

int main() {
    OneWayBlockedBarrier barrier(4);
    std::thread threads[4];
    for(uint32_t threadIndex = 0; threadIndex<4; ++threadIndex) {
        threads[threadIndex] = std::thread( threadFuncBarrier, &counters[threadIndex], &barrier);
    }
    barrier.wait();
    uint32_t totalCount = 0;
    for( uint32_t idx = 0; idx<4; ++idx) {
        totalCount += counters[idx];
    }
    return 0;
}