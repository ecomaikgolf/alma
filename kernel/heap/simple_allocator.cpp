/**
 * Simple Allocator
 *
 * From "absurponcho" work
 */

#include "heap/simple_allocator.h"
#include "kernel.h"

namespace heap {

/**
 * Construct the heap
 *
 * Can be on different chunks of physical memory (virtual memory mapping)
 */
simple_allocator::simple_allocator(uint64_t pages)
{
    auto aux = kernel::allocator.request_cont_page(pages);
    if (aux == nullptr) {
        kernel::tty.println("fatal error");
        return;
    }

    this->heap_address = aux;

    /* for each page, allocate and map them */
    void *iter = aux;
    for (uint64_t i = 0; i < pages; i++) {
        /* phys addr != virt addr*/
        kernel::translator.map((uint64_t)iter, (uint64_t)kernel::allocator.request_page());
        iter = (uint8_t *)iter + kernel::page_size;
    }

    /* calculate lenght of our heap */
    uint64_t length = pages * kernel::page_size;

    /* start and end pointer of the heap */
    this->heap_start = heap_address;
    this->heap_end   = (void *)((uint8_t *)heap_start + length);

    heap_header *start_header = (heap_header *)heap_address;
    start_header->length      = length - sizeof(heap_header);
    start_header->next        = nullptr;
    start_header->last        = nullptr;
    start_header->is_free     = true;

    this->last_header = start_header;
}

/**
 * Malloc function
 *
 * Similar to BPFA request page (more or less)
 */
void *
simple_allocator::malloc(uint64_t size)
{
    if (size == 0)
        return nullptr;

    /* round up size to ROUND_NUM (reduce fragmentation) */
    if (size % simple_allocator::ROUND_NUM != 0) {
        size -= (size % simple_allocator::ROUND_NUM);
        size += simple_allocator::ROUND_NUM;
    }

    /* iterate through the heap */
    heap_header *it = (heap_header *)heap_start;
    while (true) {
        /* if the segment is free */
        if (it->is_free) {
            /* we can split */
            if (it->length > size) {
                it->split(size);
                it->is_free = false;
                return (void *)((uint8_t *)it + sizeof(heap_header));
            }
            /* exact match*/
            if (it->length == size) {
                it->is_free = false;
                return (void *)((uint8_t *)it + sizeof(heap_header));
            }
        }

        if (it->next == nullptr)
            break;

        it = it->next;
    }
    /* no memory for size, we need to expand the heap */
    this->expand_heap(size);
    return malloc(size);
}

/**
 * Free function
 */
void
simple_allocator::free(void *addr)
{
    /* -1 to get the header addr and not the memory block */
    heap_header *header = (heap_header *)addr - 1;

    header->is_free = true;
    this->combine_forward(header);
    this->combine_backward(header);
}

/**
 * Increase heap size
 *
 * Get physical pages and map them to contiguous virtual addresses
 */
void
simple_allocator::expand_heap(uint64_t size)
{
    /* round up size to ROUND_NUM (reduce fragmentation) */
    if (size % simple_allocator::ROUND_NUM != 0) {
        size -= (size % simple_allocator::ROUND_NUM);
        size += simple_allocator::ROUND_NUM;
    }

    auto pages = size / kernel::page_size;

    heap_header *header = (heap_header *)this->heap_end;
    for (uint64_t i = 0; i < pages; i++) {
        /* phys addr != virt addr*/
        kernel::translator.map((uint64_t)this->heap_end,
                               (uint64_t)kernel::allocator.request_page());
        this->heap_end = (uint8_t *)this->heap_end + kernel::page_size;
    }

    header->is_free         = true;
    header->last            = this->last_header;
    this->last_header->next = header;
    this->last_header       = header;
    header->next            = nullptr;
    header->length          = size - sizeof(heap_header);
    this->combine_backward(header);
}

/**
 * Split a heap node
 */
simple_allocator::heap_header *
simple_allocator::heap_header::split(uint64_t size)
{
    /* too small to split */
    if (size < simple_allocator::ROUND_NUM)
        return nullptr;

    /* size of new segment */
    int64_t split_length = this->length - size - sizeof(heap_header);

    /* too small to split */
    if (split_length < simple_allocator::ROUND_NUM)
        return nullptr;

    heap_header *header = (heap_header *)((uint8_t *)this + size + sizeof(heap_header));
    /* linked list */
    this->next->last = header;
    header->next     = this->next;
    this->next       = header;
    header->last     = this;
    /* set new data */
    header->length  = split_length;
    header->is_free = true;
    this->length    = size;

    /* update last header if we *were* the last one */
    if (this->last == this)
        this->last = header;

    return header;
}

/**
 * Merge forward a heap node
 */
void
simple_allocator::combine_forward(heap_header *hdr)
{
    if (hdr->next == nullptr || !hdr->next->is_free)
        return;

    if (hdr->next == this->last_header)
        last_header = hdr;

    hdr->next = hdr->next->next;

    hdr->length = hdr->length + hdr->next->length + sizeof(heap_header);
}

/**
 * Merge backwards a heap node
 */
void
simple_allocator::combine_backward(heap_header *hdr)
{
    if (hdr->last != nullptr && hdr->last->is_free)
        this->combine_forward(hdr->last);
}

} // namespace heap
