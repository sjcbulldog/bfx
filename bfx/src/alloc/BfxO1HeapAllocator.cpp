#include <BfxO1HeapAllocator.hpp> 
#include <alloc/o1heap.h>

namespace Bfx {
cy_rslt_t BfxO1HeapAllocator::init(void *mem, size_t size) 
{
    this->heapInstance_ = o1heapInit(mem, size);
    return CY_RSLT_SUCCESS;
}
}