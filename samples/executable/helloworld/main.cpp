#include <cstdint>
#include <atomic>

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
class Node {
private:
    static_assert(sizeof(Ptr<Node<T>>) == sizeof(uint64_t), "Ptr<Node<T>> must be 8 bytes" );
    //
    std::atomic<Ptr<Node>>  _next;
    T                       _data;
public:
    Node(T&& t) : _data(std::move(t)) {}

    std::atomic<Ptr<Node>>& next() {
        return _next;
    }
    T& data() {
        return _data;
    }
};

template<class T>
class Queue {
private:
    std::atomic<Ptr<Node<T>>> _head;
    std::atomic<Ptr<Node<T>>> _tail;
public:
    Queue() {
        Node<T>* node = new Node<T>(T());
        Ptr<Node<T>> headPtr(node, 0);
        Ptr<Node<T>> tailPtr(node, 0x7fff);
        _head.store(headPtr);
        _tail.store(tailPtr);
    }

    void push(T&& t) {
        Node<T>* node = new Node<T>(std::move(t));
        while(true) {
            Ptr<Node<T>> tail = _tail.load(std::memory_order::memory_order_relaxed);
            Ptr<Node<T>> next = tail->next().load(std::memory_order::memory_order_relaxed);
            if(!next.isNull()) {
                _tail.compare_exchange_weak(tail, next, std::memory_order::memory_order_relaxed);
                continue;
            }
            Ptr<Node<T>> new_node(node, next.ver() + 1);
            Ptr<Node<T>> null(nullptr, 0);
            if(!tail->next().compare_exchange_weak(next, new_node, std::memory_order::memory_order_relaxed)) {
                continue;
            }
            if(_tail.compare_exchange_weak(tail, new_node)) {
                break;
            }
        }
    }

    int pop(T& t) {
        while(true) {
            Ptr<Node<T>> head = _head.load(std::memory_order::memory_order_relaxed);
            Ptr<Node<T>> tail = _tail.load(std::memory_order::memory_order_relaxed);
            Ptr<Node<T>> next = head->next().load(std::memory_order::memory_order_relaxed);
            if(head.addrEqual(tail)) {
                return -1;
            }
            next.upgrade();
            if(_head.compare_exchange_weak(head, next, std::memory_order::memory_order_relaxed)) {
                t = std::move(next->data());
                delete next.ptr();
                return 0;
            }
        }
    }
};

int main() {
    Queue<int> q;
    q.push(1);
    int v;
    auto rst = q.pop(v);
	return 0;
}
