#include <cstdio>
#include <bitset>
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

constexpr size_t    workerCount = 4;
ThreadWorker*       workers[workerCount];

constexpr int32_t       allocatingIntent = 100000;
std::atomic<int32_t>    allocatingActually = 0;
std::atomic<int32_t>    freedActually = 0;

class ThreadWorker {
private:
    std::queue<alloc_info>          _operations;
    std::queue<alloc_info>          _backbuffer;
    std::mutex                      _mutex;
    mutable std::condition_variable _cv;
    std::thread                     _thread;
    //
    std::vector<alloc_info>         _allocatings;
    std::vector<alloc_info>         _frees;
public:

    void swapBuffer() {
        std::unique_lock<std::mutex> lck(_mutex);
        _operations.swap(_backbuffer);
    }

    static size_t ConcurrentProc( ThreadWorker* worker ) {
        std::default_random_engine re(time(0));
        std::uniform_int_distribution<unsigned> ud(1, 5);
        std::uniform_int_distribution<unsigned> alloc_ud(25, 100);
        std::uniform_int_distribution<unsigned> worker_ud(0, workerCount-1);
        std::uniform_int_distribution<unsigned> size_ud(0, workerCount-1);
        std::uniform_int_distribution<unsigned> free_ud(25, 100);
        
        std::bitset<workerCount> notify_bitmask;
        while(allocatingActually.load(std::memory_order_relaxed) < allocatingIntent || freedActually.load(std::memory_order::memory_order_relaxed) < allocatingIntent) {
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
                ++allocatingActually;
                worker->_allocatings.push_back(alloc);
                workers[worker_index]->appendAllocInfo(alloc);
                notify_bitmask.set(worker_index, true);
            }
            int32_t free_count = (int32_t)free_ud(re);
            bool swapped = false;
            for(int32_t i = 0; i<free_count; ++i) {
                alloc_info alloc;
                if(worker->_operations.size()) {
                    alloc = worker->_operations.front();     
                    worker->_operations.pop();
                } else if(!swapped) {
                    swapped = true;
                    worker->swapBuffer();
                    --i;
                    continue;
                } else {
                    break;
                }
                free(alloc.ptr);
                ++freedActually;
                worker->_frees.push_back(alloc);
            }
        }
        return 0;
    }

    void cleanupLeftAllocations() {
        _thread = std::thread(CleanupProc, this);
    }

    static size_t CleanupProc( ThreadWorker* worker) {
        while(!worker->_operations.empty()) {
            auto alloc = worker->_operations.front();
            free(alloc.ptr);
            worker->_operations.pop();
            ++freedActually;
        }
        while(!worker->_backbuffer.empty()) {
            auto alloc = worker->_backbuffer.front();
            free(alloc.ptr);
            worker->_backbuffer.pop();
            ++freedActually;
        }
        return 0;
    }

    ThreadWorker()
        : _operations()
        , _backbuffer()
        , _mutex()
        , _thread()
        {
            _thread = std::thread(ThreadWorker::ConcurrentProc, this);
    }

    void join() {
        _thread.join();
    }

    void appendAllocInfo(alloc_info const& alloc) {
        std::unique_lock<std::mutex> lck(_mutex);
        _backbuffer.push(alloc);
    }

};


int main() {
    for(uint32_t i = 0; i<workerCount; ++i) {
        workers[i] = new ThreadWorker();
    }
    for(uint32_t i = 0; i<workerCount; ++i) {
        workers[i]->join();
    }
    for(uint32_t i = 0; i<workerCount; ++i) {
        workers[i]->cleanupLeftAllocations();
    }
    for(uint32_t i = 0; i<workerCount; ++i) {
        workers[i]->join();
    }
    return 0;
}