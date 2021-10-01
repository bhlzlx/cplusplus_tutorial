#include <memory_resource>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <list>
#include <vector>

class DebugMemoryResource : public std::pmr::memory_resource {
private:
    std::string                 _name;
    std::pmr::memory_resource*  _upstream;
    uint64_t                    _bytesAllocated;
    uint64_t                    _bytesFreed;
public:
    //
    explicit DebugMemoryResource( std::string name, std::pmr::memory_resource* upstream = std::pmr::get_default_resource() )
        : _name(name)
        , _upstream(upstream)
    {
    }
private:
    virtual void* do_allocate( size_t size, size_t alignment ) override {
        auto rst = _upstream->allocate(size, alignment);
        _bytesAllocated += size;
        std::cout<<_name<<":"<<"[Allocate] "<<size<<" bytes"<<std::endl;
        return rst;
    }

    virtual void do_deallocate(void* ptr, size_t size, size_t alignment) override {
        _upstream->deallocate(ptr, size, alignment);
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

class ComplexDataType {
private:
    size_t              _id;
    std::pmr::string    _name;
public:
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    allocator_type get_allocator() {
        return _name.get_allocator();
    }

    ComplexDataType( allocator_type allocator = {} )
        : _id(0)
        , _name(allocator)
    {
    }
    ComplexDataType( ComplexDataType&& other, allocator_type allocator = {} ) noexcept
        : _id(other._id)
        , _name( std::move(other._name), allocator )
    {
    }
    ComplexDataType( size_t id, const std::pmr::string& name, allocator_type allocator = {} )
        : _id(0)
        , _name(name, allocator)
    {}
};

constexpr size_t SizeofComplexDataType = sizeof(ComplexDataType);

DebugMemoryResource debugMemoryResource("simple debug resource");

int main() {

    std::byte stackBuffer[1024*1024]; // 1MB buffer
    std::pmr::monotonic_buffer_resource stackResource (
        std::data(stackBuffer),
        std::size(stackBuffer),
        std::pmr::null_memory_resource()
    );
    DebugMemoryResource debugMR("debug mr", &stackResource);
    std::pmr::vector<ComplexDataType>::allocator_type allocator( &debugMR );
    std::pmr::vector<ComplexDataType> v1(allocator);
    v1.emplace_back(1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    v1.emplace_back(2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    return 0;
}