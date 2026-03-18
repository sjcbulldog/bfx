#pragma once

#include "cy_result.h"
#include <cstddef>

namespace Bfx {
class Allocator {
public:
    virtual ~Allocator() = default;

    virtual cy_rslt_t init(void *mem, size_t size) = 0 ;
    virtual void* alloc(size_t size) = 0;
    virtual void free(void* ptr) = 0;
};
}
