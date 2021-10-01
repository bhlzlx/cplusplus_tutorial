#include <cstdio>
#include <unordered_map>


template< class V >
class LRUListNode {
    uint64_t        _key;
    V               _value;
    LRUListNode*    _next;
    LRUListNode*    _prev;
    //
    LRUListNode( uint64_t key, V&& value, LRUListNode<V>* prev, LRUListNode<V>* next )
        : _key(key)
        , _value(std::move(value))
        , _prev(prev)
        , _next(next)
    {
    }
    LRUListNode( uint64_t key, const V& value, LRUListNode<V>* prev, LRUListNode<V>* next )
        : _key(key)
        , _value(value)
        , _prev(prev)
        , _next(next)
    {
    }
    void setValue( V&& value ) {
        _value = std::move(value);
    }
    void setValue( const V&& value ) {
        _value = value;
    }
};

template <class T>
class LRUList {    
    LRUListNode<T>  _head;
    LRUListNode<T>  _tail;
    uint64_t        _size;
    //
    LRUList()
        : _head( 0, T(), nullptr, &_tail )
        , _tail( 0, T(), &_head, nullptr )
        , _size(0)
    {
    }
    //
    void swapToFront( LRUListNode<T>* node ) {
        auto headNext = _head._next;
        auto head = &_head;
        //
        if( node->_prev == head ) {
            return;
        }
        node->_prev->_next = node->_next;
        node->_next->_prev = node->_prev;
        //
        node->_prev = head;
        node->_next = headNext;
        //
        headNext->_prev = node;
        head._next = node;
    }

    LRUListNode<T>* insertAtFront( uint64_t key, T&& value ) {
        auto headNext = _head._next;
        auto head = &_head;
        LRUListNode<T>* node = new LRUListNode<T>(key, std::move(value), head, headNext);
        headNext->_prev = node;
        head->next = node;
        //
        ++_size;
        return node;
    }

    void insertAtFront( uint64_t key, const T& value ) {
        auto headNext = _head._next;
        auto head = &_head;
        LRUListNode<T>* node = new LRUListNode<T>(key, value, head, headNext);
        headNext->_prev = node;
        head->next = node;
        //
        ++_size;
    }
};

template< class T >
class LRUCache {
    std::unordered_map< uint64_t, LRUListNode<T>* >     _hashTable;
    LRUList<T>                                          _lruList;
    uint32_t                                            _capacity;


    LRUCache( uint32_t capacity )
        : _hashTable {}
        , _lruList()
        , _capacity(capacity) {

        }

    bool get( uint64_t key, T& value ) const 
    {
        auto iter = _hashTable.find(key);
        if(iter == _hashTable.end()) {
            return false;
        }
        LRUListNode<T>* node = iter->second;
        _lruList.swapToFront(node);
        //
        value = node->_value;
        return true;
    }
    //
    void set( uint64_t key, T&& value ) {
        auto iter = _hashTable.find(key);
        if(iter != _hashTable.end()) {
            iter->second->value = std::move(value);
            return;
        }
        if( _lruList._size == _capacity ) {
            LRUListNode<T>* last = _lruList._tail._prev();
            _hashTable.erase(last->_key);
            last->_key = key;
            last->_value = std::move(value);
            _lruList.swapToFront(last);
            _hashTable[key] = last;
            return;
        } else {
            LRUListNode<T>* node = _lruList.insertAtFront( key, std::move(value) );
            _hashTable[key] = node;
        }
        return;
    }
    //
    void set( uint64_t key, const T& value ) {
        auto iter = _hashTable.find(key);
        if(iter != _hashTable.end()) {
            iter->second->value = value;
            return;
        }
        if( _lruList._size == _capacity ) {
            LRUListNode<T>* last = _lruList._tail._prev();
            _hashTable.erase(last->_key);
            last->_key = key;
            last->_value = value;
            _lruList.swapToFront(last);
            _hashTable[key] = last;
            return;
        } else {
            LRUListNode<T>* node = _lruList.insertAtFront( key, value);
            _hashTable[key] = node;
        }
        return;
    }
};

int main() {
	return 0;
}