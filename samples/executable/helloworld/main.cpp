#include <cstdint>
#include <atomic>
#include <thread>
#include <cassert>

constexpr uint32_t CacheLineSize = 64;

template<class T>
class Ptr {
    union {
        struct {
            uint64_t addr: 48;
            uint64_t vers: 16;
        };
        uint64_t _raw;
    };
public:
    Ptr() : _raw(0) {}
    Ptr(T* ptr, uint64_t ver):
        addr(reinterpret_cast<uint64_t>(ptr)),
        vers(ver)
    {}
    //
    T* operator->() {
        return reinterpret_cast<T*>(addr);
    }
    uint64_t ver() const {
        return vers;
    }
    void upgrade() {
        vers++;
    }
    void setPtr(T* ptr) {
        addr = reinterpret_cast<uint64_t>(ptr);
    }
    void setVer(uint64_t ver) {
        vers = ver;
    }
    T* ptr() {
        return reinterpret_cast<T*>(addr);
    }
    bool addrEqual( const Ptr<T>& other ) const {
        return addr == other.addr;
    }
    bool operator == (const Ptr& other) const {
        return _raw == other._raw;
    }
    bool isNull() const {
        return addr == 0;
    }
    const uint64_t& raw() const {
        return _raw;
    }
};


template<class T>
class alignas(8) Node {
private:
    static_assert(sizeof(Ptr<Node<T>>) == sizeof(uint64_t), "Ptr<Node<T>> must be 8 bytes" );
    //
    std::atomic<Ptr<Node>>  _next;
    std::atomic<T>          _data;
public:
    Node(T&& t) 
        : _next(Ptr<Node>(nullptr, 0))
        , _data(std::move(t)) {
    }

    std::atomic<Ptr<Node>>& next() {
        return _next;
    }
    T data() {
        return _data.load(std::memory_order::memory_order_relaxed);
    }
};

template<class T>
class Queue {
private:
    alignas(CacheLineSize) std::atomic<Ptr<Node<T>>> _head;
    alignas(CacheLineSize) std::atomic<Ptr<Node<T>>> _tail;
public:
    Queue() {
        Node<T>* node = new Node<T>(T());
        Ptr<Node<T>> headPtr(node, 0);
        Ptr<Node<T>> tailPtr(node, 0x7fff);
        _head.store(headPtr);
        _tail.store(tailPtr);
    }

    void push(T&& t) {
        Ptr<Node<T>> node_ptr(new Node<T>(std::move(t)), 0);
        while(true) {
            Ptr<Node<T>> tail = _tail.load(std::memory_order::memory_order_relaxed);
            auto& tail_next = tail.ptr()->next();
            Ptr<Node<T>> next = tail_next.load(std::memory_order::memory_order_relaxed);
            if(!next.isNull()) {
                continue;
            }
            node_ptr.setVer(next.ver() + 1);
            node_ptr.ptr()->next().store(Ptr<Node<T>>(nullptr, next.ver()+1), std::memory_order::memory_order_relaxed);
            if(!tail_next.compare_exchange_weak(next, node_ptr, std::memory_order::memory_order_relaxed)) {
                continue;
            }
            auto rst = _tail.compare_exchange_strong(tail, node_ptr, std::memory_order::memory_order_relaxed);
            assert(rst);
            break;
        }
    }

    int pop(T& t) {
        while(true) {
            Ptr<Node<T>> head = _head.load(std::memory_order::memory_order_relaxed);
            Ptr<Node<T>> tail = _tail.load(std::memory_order::memory_order_relaxed);
            Node<T>* head_ptr = head.ptr();
            Ptr<Node<T>> next = head_ptr->next().load(std::memory_order::memory_order_consume);
            if(head.addrEqual(tail) || next.isNull()) {
                return -1;
            }
            next.upgrade();
            if(_head.compare_exchange_weak(head, next, std::memory_order::memory_order_release, std::memory_order::memory_order_relaxed)) {
                Node<T>* next_ptr = next.ptr();
                t = next_ptr->data();
                delete head_ptr;
                return 0;
            }
        }
    }
};

struct alignas(CacheLineSize) producer_t {
    uint64_t counter;
};

struct alignas(CacheLineSize) consumer_t {
    uint64_t sum;
};

struct TaskConext {
    producer_t producer_info[8];
    consumer_t consumer_info[8];
};

size_t produce_proc(TaskConext* context, size_t id, Queue<uint64_t>& queue){
    uint64_t i = 0;
    while(i<240000) {
        queue.push(std::move(i));
        // context->producer_info[id].counter+=i;
        context->producer_info[id].counter+=i;
        ++i;
    }
    return 0;
}

size_t consumer_proc(TaskConext* context, size_t id, Queue<uint64_t>& queue){
    uint64_t i = 0;
    while(i<120000) {
        uint64_t data;
        if(queue.pop(data) == 0) {
            context->consumer_info[id].sum+=data;
            i++;
        }
    }
    return 0;
}

TaskConext taskContext = { {}, {} };

int main() {
    Queue<uint64_t> q;

    std::thread producer_threads[] = {
        std::thread(produce_proc, &taskContext, 0, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 1, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 2, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 3, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 4, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 5, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 6, std::ref(q)),
        // std::thread(produce_proc, &taskContext, 7, std::ref(q))
    };
    std::thread consumer_threads[] = {
        std::thread(consumer_proc, &taskContext, 0, std::ref(q)),
        std::thread(consumer_proc, &taskContext, 1, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 2, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 3, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 4, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 5, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 6, std::ref(q)),
        // std::thread(consumer_proc, &taskContext, 7, std::ref(q))
    };

    for(auto& t : producer_threads) {
        t.join();
    }
    for(auto& t : consumer_threads) {
        t.join();
    }
	return 0;
}
