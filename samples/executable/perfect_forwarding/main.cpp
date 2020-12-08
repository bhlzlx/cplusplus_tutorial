#include <cstdio>
#include <cstdint>
#include <utility>
#include <memory>

typedef uint64_t ResourceHandle;

extern ResourceHandle createExpensiveResource( size_t _size );
extern void destroyExpensiveResource( ResourceHandle _res );
extern ResourceHandle copyExpensiveResource( ResourceHandle _res );

class LowLevelValue {
private:
    ResourceHandle m_resource;
    size_t m_size;
public:
    // 默认构造函数
    LowLevelValue( ResourceHandle _handle, size_t _size ) 
        : m_resource( _handle )
        , m_size( _size )
    {
    }

    // 移动构造函数
    LowLevelValue( LowLevelValue&& _v ) {
        m_resource = _v.m_resource;
        m_size = _v.m_size;
        _v.m_resource = 0;
        _v.m_size = 0;
    }

    // 复制构造函数
    LowLevelValue( const LowLevelValue& _v ) {
        m_resource = copyExpensiveResource( _v.m_resource );
        m_size = _v.m_size;
    }
};

class HighLevelValue {
private:
    LowLevelValue m_res;
public:
    HighLevelValue( LowLevelValue&& _value )
        :m_res(std::move(_value))
    {
    }

    HighLevelValue( const LowLevelValue& _value )
        :m_res(_value)
    {
    }
};

template< class T, class S > 
class Factory {
public:
    //T createObject( S _arg ) {
    //    return T(_arg);
    //}
    // T createObject( const S &  _arg ) {
    //     return T(_arg);
    // }
    T createObject( S &&  _arg ) {
        return T(std::forward<S>(_arg));
    }
};

// template< class T, class ...ARGS > 
// class Factory {
// public:
//     T createObject( ARGS && ..._args ) {
//         return T( std::forward<ARGS>(_args)... );
//     }
//     T createObject( ARGS ..._args ) {
//         return T( _args ... );
//     }
// };

LowLevelValue createLowLevelValue(){
    return LowLevelValue ( createExpensiveResource(1024), 1024 );
}

int main() {
    LowLevelValue v1( createExpensiveResource(1024), 1024 );
    LowLevelValue v2 = v1;
    LowLevelValue v3 = std::move(v1);
    printf("%p %p\n", &v2, &v3);
    Factory<HighLevelValue,LowLevelValue> factory;
    factory.createObject(createLowLevelValue());
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