#include <cstdio>
#include <cstdint>
#include <utility>
#include <memory>

typedef uint64_t ResourceHandle;

extern ResourceHandle createExpensiveResource( size_t _size );
extern void destroyExpensiveResource( ResourceHandle _res );
extern ResourceHandle copyExpensiveResource( ResourceHandle _res );

class Value {
private:
    ResourceHandle m_resource;
    size_t m_size;
public:
    // 默认构造函数
    Value( ResourceHandle _handle, size_t _size ) 
        : m_resource( _handle )
        , m_size( _size )
    {
    }

    // 移动构造函数
    Value( Value&& _v ) {
        m_resource = _v.m_resource;
        m_size = _v.m_size;
        _v.m_resource = 0;
        _v.m_size = 0;
    }

    // 复制构造函数
    Value( const Value& _v ) {
        m_resource = copyExpensiveResource( _v.m_resource );
        m_size = _v.m_size;
    }
};

template< class T, class ...ARGS > 
class Factory {
public:
    T createObject( ARGS && ..._args ) {
        return T( std::forward<ARGS>(_args)... );
    }
    T createObject( ARGS ..._args ) {
        return T( _args ... );
    }
};

int main() {
    Value v1( createExpensiveResource(1024), 1024 );
    Value v2 = v1;
    Value v3 = std::move(v1);
    printf("%p %p\n", &v2, &v3);
    Factory<Value, ResourceHandle, size_t> factory;
    ResourceHandle a = 0; size_t b = 0;
    factory.createObject( a, b );
    return 0;
}

ResourceHandle createExpensiveResource( size_t _size ) {
    return 0xbeef;
}

ResourceHandle copyExpensiveResource( ResourceHandle _res ) {
    return 0xbeef;
}

void destroyExpensiveResource( ResourceHandle _res ) {
    return;
}