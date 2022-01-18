#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <queue>
#include <cstdint>

#include <random>

/**
 * @brief 
 * allocated memory info
 */
struct alloc_info {
    void*       ptr;
    uint32_t    size; 
};

class ThreadWorker;

ThreadWorker*       workers = nullptr;
size_t              workerCount = 4;

class ThreadWorker {
private:
    std::vector<alloc_info>         _operations;
    std::vector<alloc_info>         _backbuffer;
    std::mutex                      _mutex;
    std::condition_variable         _cv;
    std::thread                     _thread;
    size_t                          _allocCount;
public:
    ThreadWorker( size_t allocCount )
        : _operations()
        , _backbuffer()
        , _mutex()
        , _thread()
        , _allocCount(allocCount){
    }

    void join() {
        _thread.join();
    }

    void appendAllocInfo(alloc_info const& alloc) {
        std::unique_lock<std::mutex> lck(_mutex);
        _backbuffer.push_back(alloc);
    }

    static size_t ThreadProc( ThreadWorker* worker ) {
        std::default_random_engine re(time(0));
        std::uniform_int_distribution<unsigned> ud(1, 5);
        std::uniform_int_distribution<unsigned> alloc_ud(25, 100);
        std::uniform_int_distribution<unsigned> worker_ud(0, workerCount-1);
        std::uniform_int_distribution<unsigned> size_ud(0, workerCount-1);
        while(worker->_allocCount) { 
            
            using ms = std::chrono::duration<uint32_t, std::milli>;
            auto sleep_time = ms(ud(re)); // 1~5 ms
            std::this_thread::sleep_for(sleep_time);
            auto alloc_count = alloc_ud(re);
            for(uint32_t i = 0; i<alloc_count; ++i) {
                auto worker_index = worker_ud(re);
                auto alloc_size = worker_ud(re);
                alloc_info alloc = {
                    malloc(alloc_size),
                    alloc_size
                };
                workers[i].appendAllocInfo(alloc);
            }
        }
        return 0;
    }

};


int main() {
    return 0;
}