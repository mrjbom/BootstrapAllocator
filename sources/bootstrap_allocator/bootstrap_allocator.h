#ifndef _BOOTSTRAP_ALLOCATOR_H_
#define _BOOTSTRAP_ALLOCATOR_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * Bootstrap Allocator
 * A simple and primitive allocator that uses a bitmap to pages allocation.
 * It is focused on working in the OS kernel to initialize the memory required to configure the main allocator and other things (page tables).
 * The bitmap memory is reserved in advance for 4 GB of memory.
 * It is focused on use in a 32-bit address space.
 */

#define BOOTSTRAP_ALLOCATOR_MAX_MEMORY_SIZE 4294967296ll

#define BOOTSTRAP_ALLOCATOR_PAGE_SIZE 4096

#define BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER BOOTSTRAP_ALLOCATOR_MAX_MEMORY_SIZE / BOOTSTRAP_ALLOCATOR_PAGE_SIZE

// Bitmap size in bytes
// 1 bit per page it's 1 byte per 8 pages
#define BOOTSTRAP_ALLOCATOR_BITMAP_SIZE BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER / 8

extern uint32_t bootstrap_allocator_min_pfn;
extern uint32_t bootstrap_allocator_max_pfn;
extern uint8_t bootstrap_allocator_bitmap[];

/*
 * Inits bootstrap allocator
 * Mark all pages all allocated by default!
 * min_pfn - index of the initial page controlled by the allocator must be greater than 0.
 * max_pfn - index of the final page controlled by the allocator.
 * [min_pfn; max_pfn]
 * Use mark_range for setting up
 */
extern void bootstrap_allocator_init(uint32_t min_pfn, uint32_t max_pfn);

/*
 * Marks pages starting with addr and ending with addr+size as free or used
 * 0 - free
 * 1 - used
 * 
 * Examples:
 * mark_range(0x2000, 16384, 0)
 * Marks 4 pages as free: 0x2000, 0x3000, 0x4000, 0x5000
 * mark_range(0x2000, 17000, 1)
 * Marks 5 pages as used: 0x2000, 0x3000, 0x4000, 0x5000, 0x6000
 * mark_range(0x1FFF, 16384, 0)
 * Marks 4 pages as free: 0x1000, 0x2000, 0x3000, 0x4000
 */
extern void bootstrap_allocator_mark_range(void* addr, uintptr_t size, bool state);

/*
 * Allocates one or more pages depending on the size
 * Returns NULL if failed
 */
extern void* bootstrap_allocator_alloc_pages(size_t size);

#endif
