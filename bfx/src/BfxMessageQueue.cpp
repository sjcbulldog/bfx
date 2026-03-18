#include <cy_result.h>
#include <BfxMessageQueue.hpp>
#include <BfxErrors.hpp>
#include <BfxAllocator.hpp> 

namespace Bfx {
MessageQueue::MessageQueue(Allocator *allocator, size_t count, size_t item_size)
{
    this->heapinst_ = allocator ;
    this->count_ = count ;
    this->item_size_ = item_size ;
} 

cy_rslt_t MessageQueue::init(bool isFirst, uint32_t semno)
{
    return isFirst ? initFirst(semno) : initAfter(semno) ;
}

cy_rslt_t MessageQueue::initFirst(uint32_t semno)
{
    cy_rslt_t rslt = CY_RSLT_SUCCESS ;
    mtb_ipc_shared_t *shared ;

    // Allocated shared memory for the IPC object and the queue pool from the provided heap instance. 
    // The shared memory is used by both sides of the IPC communication, so it must be allocated from a shared heap.
    shared = (mtb_ipc_shared_t *)this->heapinst_->alloc(sizeof(mtb_ipc_shared_t)) ;
    if (shared == nullptr) {
        return BFX_QUEUE_NO_MEMORY ;
    }

    // The queue pool is used to store the items in the queue. It must be allocated from the shared heap because it 
    // is accessed by both sides of the IPC communication.
    void *pool = this->heapinst_->alloc(count_ * item_size_) ;
    if (pool == nullptr) {
        this->heapinst_->free(shared) ;
        return BFX_QUEUE_NO_MEMORY ;
    }

    mtb_ipc_config_t configobj = {
        .internal_channel_index = MTB_IPC_CHAN_0,
        .semaphore_irq = MTB_IPC_IRQ_USER,
        .queue_irq = MTB_IPC_IRQ_USER + 1,
        .semaphore_num = semno,
    } ;

    mtb_ipc_queue_config_t config = {
        .channel_num = MTB_IPC_CHAN_1,
        .queue_num = 1,
        .max_num_items = 4,
        .item_size = 4, 
        .queue_pool = pool,
        .semaphore_num = semno
    } ;

    rslt = mtb_ipc_init(&obj_, shared, &configobj) ;
    if (rslt != CY_RSLT_SUCCESS) {
        this->heapinst_->free(pool) ;
        this->heapinst_->free(shared) ;
        return rslt ;
    }


    rslt = mtb_ipc_queue_init(&obj_, &queue_, queue_obj_, &config) ;
    if (rslt != CY_RSLT_SUCCESS) {
        this->heapinst_->free(pool) ;
        this->heapinst_->free(shared) ;
        return rslt ;
    }

    return CY_RSLT_SUCCESS ;
}

cy_rslt_t MessageQueue::initAfter(uint32_t semno)
{
    return CY_RSLT_SUCCESS ;
}
}
