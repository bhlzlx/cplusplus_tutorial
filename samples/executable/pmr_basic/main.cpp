#include <memory_resource>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <list>
#include <vector>

#include "GxDSA.h"

class DebugMemoryResource : public std::pmr::memory_resource {
private:
    std::string                 _name;
    std::pmr::memory_resource*  _upstream;
    GxDSA                       _dsa;
    uint64_t                    _bytesAllocated;
    uint64_t                    _bytesFreed;
public:
    explicit DebugMemoryResource( std::string name, std::pmr::memory_resource* upstream = std::pmr::get_default_resource() )
        : _name(name)
        , _upstream(upstream)
    {
    }
private:
    virtual void* do_allocate( size_t size, size_t alignment ) override {
        auto rst = _dsa.allocateAligned(size, alignment);
        _bytesAllocated += size;
        std::cout<<_name<<":"<<"[Allocate] "<<size<<" bytes"<<std::endl;
        return rst;
    }

    virtual void do_deallocate(void* ptr, size_t size, size_t alignment) override {
        _dsa.deallocate(ptr);
        _bytesFreed += size;
        std::cout<<_name<<":"<<"[Deallocate] "<<size<<" bytes"<<std::endl;
    }
    // https://en.cppreference.com/w/cpp/memory/memory_resource/do_is_equal
    /*Two memory_resources compare equal if and only if memory allocated from one memory_resource can be deallocated from the other and vice versa.*/
    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
        if( &other == this ) {
            return true;
        }
        return false;
    }
};

struct PlainOldDataType {
    size_t data1;
    size_t data2;
};

DebugMemoryResource debugMemoryResource("TLSF debug resource1");
// DebugMemoryResource debugMemoryResource2("TLSF debug resource2");
    template<class T>
    using pmr_vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
    template<class T>
    using pmr_list = std::list<T, std::pmr::polymorphic_allocator<T>>;

    template < class char_type, class traits_type = std::char_traits<char_type>, class PMR = std::pmr::polymorphic_allocator<char_type> >
    using pmr_basic_string = std::basic_string<char_type, traits_type, PMR >;
    using pmr_string = pmr_basic_string<char>;

int main() {
    // 栈内分配器示例
    {
        uint8_t buffer[2048];
        std::pmr::monotonic_buffer_resource stackPool( std::data(buffer), std::size(buffer), &debugMemoryResource);
        pmr_list<char> charList( &stackPool );
        for( uint32_t i = 0; i<('z'-'a'); ++i) {
            charList.push_back('a' + i);
        }
        // 加断点，使用调试器观察buffer的内容
        system("pause");  // GDB:: -exec x/256c buffer
    }
    //
    {
        pmr_vector<PlainOldDataType> vecTest{ {{},{},{},{},{},{},{},{},{} }, &debugMemoryResource};
        pmr_vector<PlainOldDataType> vecTest1(std::move(vecTest), &debugMemoryResource);
    }

    return 0;
}