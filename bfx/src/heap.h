#pragma once

#include <BfxConfig.hpp>

#if defined(BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY) && (BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY == BFX_USE_O1HEAP_ALLOCATOR)
#include "o1heap.h"

inline void *heapInit(void* const heap_memory, const size_t heap_size) {
    // The heap instance is stored at the beginning of the heap memory, so the user code can only operate on the pointer.
    return o1heapInit(heap_memory, heap_size) ;
}

void *heapAllocate(void *inst, const size_t amount) {
    return o1heapAllocate((O1HeapInstance*) inst, amount) ;
}

void heapFree(void *inst, void* const pointer)  {
    o1heapFree((O1HeapInstance*) inst, pointer) ;
}


#elif defined(BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY) && (BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY == BFX_USE_SIMPLE_ALLOCATOR)
#include "simple.h"

inline void heapInit(void* const heap_memory, const size_t heap_size);
void *heapAllocate(const size_t amount);
void heapFree(void* const pointer);


#else
#error "Invalid BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY"
#endif

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
