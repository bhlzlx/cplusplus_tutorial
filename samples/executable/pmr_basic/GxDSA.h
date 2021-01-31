#pragma once
#include "tlsf/tlsf.h"
#include <cstdlib>
#include <cassert>

class GxDSA {
    constexpr static size_t DefaultPoolSize = 2 * 1024 * 1024;
private:
    tlsf_t      _tlsf;
    size_t      _runtimeSize;
    size_t      _usedSize;
public:
    GxDSA();
    ~GxDSA();

    void* allocate( size_t size );

    void* allocateAligned( size_t size, size_t alignment );

    void deallocate( void* ptr );

    void reset();

};