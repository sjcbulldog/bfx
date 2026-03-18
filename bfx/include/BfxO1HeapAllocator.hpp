#pragma once

#include "BfxAllocator.hpp"

struct O1HeapInstance;

namespace Bfx {
class BfxO1HeapAllocator : public Allocator {
public:
    BfxO1HeapAllocator() = default;
    ~BfxO1HeapAllocator() = default;

    cy_rslt_t init(void *mem, size_t size);
    void* allocate(size_t size);
    void deallocate(void* ptr);

private:
    O1HeapInstance *heapInstance_ ;

};  
}
