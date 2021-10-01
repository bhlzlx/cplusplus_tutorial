#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include "../../utility/Timer.h"

class Barrier {
private:
    mutable std::mutex                              _mutex;
    mutable std::condition_variable                 _conditionVariable;
    mutable uint32_t                                _waitCount;
    mutable std::atomic<uint32_t>                   _counter;
    mutable uint32_t                                _round;
public:
    Barrier( uint32_t waitCount )
        : _mutex()
        , _conditionVariable()
        , _waitCount(waitCount)
        , _counter(0)
    {}

    void wait() const {
        std::unique_lock<std::mutex> lock(_mutex);
        auto round = _round;
        ++_counter;
        if(_counter != _waitCount) {
            _conditionVariable.wait(lock, [this,&round]()->bool {
                return round != _round;
            });
        }
        else {
            ++_round;
            _conditionVariable.notify_all();
        }
    }

    void reset() const {
        std::lock_guard<std::mutex> lock(_mutex);
        ++_round;
        _counter = 0;
        _conditionVariable.notify_all();
    }
};


class Barrier2 {
private:
    mutable std::mutex                              _mutex;
    mutable std::condition_variable                 _conditionVariable;
    mutable uint32_t                                _waitCount;
    mutable std::atomic<uint32_t>                   _counter;
    mutable uint32_t                                _round;
public:
    Barrier2( uint32_t waitCount)
        : _mutex()
        , _conditionVariable()
        , _waitCount(waitCount)
        , _counter(0)
    {}

    void sig() const {
        std::unique_lock<std::mutex> lock(_mutex);
        ++_counter;
        if(_counter == _waitCount) {
            ++_round;
            _conditionVariable.notify_all();
        }
    }

    void wait() const {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_counter != _waitCount) {
            _conditionVariable.wait(lock, [this]()->bool {
                return _counter == _waitCount;
            });
        }
    }

    void reset() const {
        std::lock_guard<std::mutex> lock(_mutex);
        ++_round;
        _counter = 0;
        _conditionVariable.notify_all();
    }
};

class Semaphore {
private:
    mutable std::mutex                  _mutex;
    mutable std::condition_variable     _conditionVariable;
    mutable std::atomic<uint32_t>       _counter;
public:
    Semaphore( uint32_t counter )
        : _mutex()
        , _conditionVariable()
        , _counter(0)
    {}

    void wait() const {
        std::unique_lock<std::mutex> lock(_mutex);
        /**
        * @brief 为什么wait(lock, predator ) 后边这个Predator是必要的，
        * 因为Linux上应用会出现中断，在wait的时候中断会唤醒线程。这个时候
        * 并没有notify，所以要加一个predator。
        */
        if(_counter == 0) {
           _conditionVariable.wait(lock, [&]()->bool {
                return _counter != 0;
            }); // 阻塞
        }
        else {
            --_counter; // 不阻塞，正常通过
        }
    }

    void sig() const {
        std::unique_lock<std::mutex> lock(_mutex);
        ++_counter;
        _conditionVariable.notify_one();
    }
};

uint32_t counters[4];

void threadFuncBarrier( uint32_t* counter, Barrier2* barrier ) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 假设算法耗时1秒
    barrier->sig();
}

int main() {
    Barrier2 barrier(4);
    std::thread threads[4];

    // 创建4个任务，并发执行，总耗时也就一秒多一些。
    for(uint32_t threadIndex = 0; threadIndex<4; ++threadIndex) {
        threads[threadIndex] = std::thread( threadFuncBarrier, &counters[threadIndex], &barrier);
    }
    cpp::Timer timer;
    barrier.wait();
    uint32_t totalCount = 0;
    for( uint32_t idx = 0; idx<4; ++idx) {
        totalCount += counters[idx];
    }
    uint32_t multiThreadDua = timer.duration();
    printf("%u\n", multiThreadDua);

    for( auto& thread: threads) {
        thread.join();
    }
    return 0;
}