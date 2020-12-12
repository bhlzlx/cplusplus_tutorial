#include "GxDSA.h"

GxDSA::GxDSA()
    : _tlsf()
    , _runtimeSize(0)
    , _usedSize(0)
{
    size_t tlsfSize = tlsf_size();
#ifdef _WIN32 // Window API
    void* mem = _aligned_malloc(tlsfSize, 8);
#else   // POSIX API
    void* mem = memalign(8, tlsfSize);
#endif
    _tlsf = tlsf_create(mem);
}

void* GxDSA::allocate( size_t size ) {
    void* mem = tlsf_malloc(_tlsf, size );
    if(!mem) {
        if(size <= DefaultPoolSize) {
            void* mem = malloc(DefaultPoolSize);
            auto pool = tlsf_add_pool(_tlsf, mem, DefaultPoolSize);
            _runtimeSize += DefaultPoolSize;
        } else {
            size_t alignedSize = (size + 1024 * 1024-1) & (1024 * 1024-1);
            auto pool = tlsf_add_pool(_tlsf, mem, alignedSize);
            _runtimeSize += alignedSize;
        }
        mem = tlsf_malloc(_tlsf, size);
        assert(mem != nullptr);
    }
    _usedSize += size;
    return mem;
}

void* GxDSA::allocateAligned( size_t size, size_t alignment ) {
    void* mem = tlsf_memalign( _tlsf, alignment, size );
    if(!mem) {
        if( size + alignment <= DefaultPoolSize) {
            void* mem = malloc(DefaultPoolSize);
            auto pool = tlsf_add_pool(_tlsf, mem, DefaultPoolSize);
            _runtimeSize += DefaultPoolSize;
        } else {
            size_t alignedSize = ((size+alignment) + 1024 * 1024-1) & (1024 * 1024-1);
            auto pool = tlsf_add_pool(_tlsf, mem, alignedSize);
            _runtimeSize += alignedSize;
        }
        mem = tlsf_memalign(_tlsf, alignment, size);
        assert(mem != nullptr);
    }
    _usedSize += size;
    return mem;
}

void GxDSA::deallocate( void* ptr ) {
    tlsf_free(_tlsf, ptr);
}

void GxDSA::reset() {
    tlsf_destroy(_tlsf);
    tlsf_create((void*)_tlsf);
}

GxDSA::~GxDSA() {
    tlsf_destroy(_tlsf);
#ifdef _WIN32
    _aligned_free(_tlsf);
#else
    free(_tlsf);
#endif
}