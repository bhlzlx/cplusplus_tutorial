#include <memory_resource>
#include <vector>

class DSAMemoryResource : std::pmr::memory_resource {
private:
    std::pmr::memory_resource*          _upstream;
public:
    DSAMemoryResource()
    {}

    void* allocate(size_t size, size_t alignment ) {

    }

    void deallocate( void* ptr, size_t size, size_t alignment ) {

    }

    bool is_equal(const memory_resource& other ) const noexcept {
        
    }
};

int main() {
    return 0;
}