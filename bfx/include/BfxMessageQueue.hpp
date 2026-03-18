#pragma once

#include <mtb_ipc.h>
#include <cy_result.h>
namespace Bfx {

class Allocator ;
class MessageQueue {
public:
    MessageQueue(Allocator *allocator, size_t count, size_t item_size) ;

    cy_rslt_t init(bool isFirst = false, uint32_t semno = 1) ;

private:
    cy_rslt_t initFirst(uint32_t semno) ;
    cy_rslt_t initAfter(uint32_t semno) ;

private:
    Allocator *heapinst_ ;
    size_t count_ ;
    size_t item_size_ ;
    mtb_ipc_queue_t queue_ ;
    mtb_ipc_queue_data_t *queue_obj_ ;
    mtb_ipc_t obj_ ;

} ;
}
