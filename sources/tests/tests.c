#include "tests.h"
#include "../bootstrap_allocator/bootstrap_allocator.h"
#include <assert.h>

void tests_mark_range()
{
    // TEST 1
    // 5 pages, 0x1000 - 0x5000
    bootstrap_allocator_init(1, 5, 0, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    // Mark 5 pages as FREE [0x1000, 0x5000]
    bootstrap_allocator_mark_range((void*)0x1000, 5 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    // Bitmap is 10000011...
    assert(bootstrap_allocator_bitmap[0] == 0b10000011);
    // Mark 3 pages as USED [0x2000, 0x4000]
    bootstrap_allocator_mark_range((void*)0x2000, 3 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    // Bitmap is 10111011...
    assert(bootstrap_allocator_bitmap[0] == 0b10111011);
    // Mark 6 pages as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0x1000, 6 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10111011);
    // Mark 0 pages as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0x1000, 0, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10111011);

    // TEST 2
    // 12 pages, 0x1000 - 0xC000
    bootstrap_allocator_init(1, 4, 5, 12);
    // Mark 5 pages as FREE [0x1000 - 0x5000]
    bootstrap_allocator_mark_range((void*)0x1000, 5 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    // Bitmap is 10000011 11111...
    // Mark 3 pages as FREE [0x6000 - 0x8000]
    bootstrap_allocator_mark_range((void*)0x6000, 3 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    // Bitmap is 10000000 01111...
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 2 pages as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0xC000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 0xC000 as FREE
    bootstrap_allocator_mark_range((void*)0xC000, 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01110111);
    // Mark 2 pages [0xB000 - 0xC0000] as USED
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 7 pages [0x1000 - 0x7000] as USED
    bootstrap_allocator_mark_range((void*)0x1000, 7 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 1 page 0x8000 as USED
    bootstrap_allocator_mark_range((void*)0x8000, 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 2 pages [0x1000, 0x2000] as FREE
    bootstrap_allocator_mark_range((void*)0x1000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10011111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 2 pages [0xB000, 0xC000] as FREE
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10011111);
    assert(bootstrap_allocator_bitmap[1] == 0b11100111);
    // Mark 3 pages [0x1000, 0x3000] as USED
    bootstrap_allocator_mark_range((void*)0x1000, 3 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11100111);
    // Mark 4 pages [0x9000, 0xC000] as FREE
    bootstrap_allocator_mark_range((void*)0x9000, 4 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b10000111);
    // Mark 12 pages [0x1000, 0xC000] as FREE
    bootstrap_allocator_mark_range((void*)0x1000, 12 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b00000111);
    // Mark 2 pages [0x1000, 0x2000] as USED
    bootstrap_allocator_mark_range((void*)0x1000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11100000);
    assert(bootstrap_allocator_bitmap[1] == 0b00000111);
    // Mark 2 pages [0xB000, 0xC000] as USED
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11100000);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
    // Mark 8 pages [0x3000, 0xA000] as USED
    bootstrap_allocator_mark_range((void*)0x3000, 8 * 4096, BOOTSTRAP_ALLOCATOR_STATE_USED);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 0 page as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0x0, 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 13 pages as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0x0, 13 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark page 13 as FREE (Fail)
    bootstrap_allocator_mark_range((void*)0xD000, 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 4 pages [0x1000, 0x4000] as FREE
    bootstrap_allocator_mark_range((void*)0x1FFF, 4 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10000111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 5 pages [0x6000, 0xA000] as FREE
    bootstrap_allocator_mark_range((void*)0x6000, 4 * 4096 + 64, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    assert(bootstrap_allocator_bitmap[0] == 0b10000100);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
}

void tests_alloc_pages(void)
{
    // TEST 1 (DMA only)
    // 8 pages DMA: 0x1000 - 0x8000
    bootstrap_allocator_init(1, 8, 0, 0);
    // 111111111
    // Alloc 1 page (Fail)
    void* allocated_ptr = bootstrap_allocator_alloc_pages(1, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    bootstrap_allocator_mark_range((void*)0x1000, 8 * 4096, 0);
    // 10000000
    // Alloc 1 page in NORMAL (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(1, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
    assert(allocated_ptr == NULL);
    // 10000000
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    
    // Alloc 9 pages (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(9 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    // Alloc 1 page
    allocated_ptr = bootstrap_allocator_alloc_pages(4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 1 * 4096);
    // 110000000
    assert(bootstrap_allocator_bitmap[0] == 0b11000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Alloc 3 pages
    allocated_ptr = bootstrap_allocator_alloc_pages(3 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 2 * 4096);
    // 111110000
    assert(bootstrap_allocator_bitmap[0] == 0b11111000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);

    // Mark 0x1000 - 0x3000 as FREE
    bootstrap_allocator_mark_range((void*)0x1000, 3 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    // 100010000
    assert(bootstrap_allocator_bitmap[0] == 0b10001000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);

    // 100010000
    // Alloc 5 pages (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(5 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    assert(bootstrap_allocator_bitmap[0] == 0b10001000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // 100010000
    // Alloc 4 pages
    allocated_ptr = bootstrap_allocator_alloc_pages(4 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 5 * 4096);
    // 100011111
    assert(bootstrap_allocator_bitmap[0] == 0b10001111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Alloc 4 pages (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(4 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    // 100011111
    assert(bootstrap_allocator_bitmap[0] == 0b10001111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Alloc 2 pages
    allocated_ptr = bootstrap_allocator_alloc_pages(2 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 1 * 4096);
    // 111011111
    assert(bootstrap_allocator_bitmap[0] == 0b11101111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Alloc 2 pages (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(2 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    // 111011111
    assert(bootstrap_allocator_bitmap[0] == 0b11101111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Alloc 1 page
    allocated_ptr = bootstrap_allocator_alloc_pages(1 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 3 * 4096);
    // 111111111
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Alloc 1 page (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(1 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);

    // TEST 2 (DMA + NORMAL)
    // 10 pages DMA: 0x1000 - 0xA000
    // 10 pages NORMAL: 0xB000 - 0x14000
    bootstrap_allocator_init(1, 10, 11, 20);
    // 11111111111|1111111111
    // 11111111 111|11111 11111...
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    assert(bootstrap_allocator_bitmap[2] == 0b11111111);
    // Mark all pages as FREE
    bootstrap_allocator_mark_range((void*)0x1000, 20 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);
    // 10000000000|0000000000
    // 10000000 000|00000 00000...
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b00000000);
    assert(bootstrap_allocator_bitmap[2] == 0b00000111);

    // Alloc 3 pages in NORMAL
    allocated_ptr = bootstrap_allocator_alloc_pages(3 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
    assert(((uintptr_t)allocated_ptr == 11 * 4096) && ((uintptr_t)allocated_ptr == 0xB000));
    // 10000000000|1110000000
    // 10000000 000|11100 00000...
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b00011100);
    assert(bootstrap_allocator_bitmap[2] == 0b00000111);
    // Alloc 2 pages in DMA
    allocated_ptr = bootstrap_allocator_alloc_pages(2 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 1 * 4096);
    // 11100000000|1110000000
    // 11100000 000|11100 00000...
    assert(bootstrap_allocator_bitmap[0] == 0b11100000);
    assert(bootstrap_allocator_bitmap[1] == 0b00011100);
    assert(bootstrap_allocator_bitmap[2] == 0b00000111);
    // Alloc 4 pages in DMA
    allocated_ptr = bootstrap_allocator_alloc_pages(4 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 3 * 4096);
    // 11111110000|1110000000
    // 11111110 000|11100 00000...
    assert(bootstrap_allocator_bitmap[0] == 0b11111110);
    assert(bootstrap_allocator_bitmap[1] == 0b00011100);
    assert(bootstrap_allocator_bitmap[2] == 0b00000111);
    // Alloc 5 pages in DMA (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(5 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert(allocated_ptr == NULL);
    // 11111110000|1110000000
    // 11111110 000|11100 00000...
    assert(bootstrap_allocator_bitmap[0] == 0b11111110);
    assert(bootstrap_allocator_bitmap[1] == 0b00011100);
    assert(bootstrap_allocator_bitmap[2] == 0b00000111);
    // Alloc 5 pages in NORMAL
    allocated_ptr = bootstrap_allocator_alloc_pages(5 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
    assert((uintptr_t)allocated_ptr == 14 * 4096);
    // 11111110000|1111111100
    // 11111110 000|11111 11100...
    assert(bootstrap_allocator_bitmap[0] == 0b11111110);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
    assert(bootstrap_allocator_bitmap[2] == 0b11100111);
    // Alloc 3 pages in NORMAL (Fail)
    allocated_ptr = bootstrap_allocator_alloc_pages(3 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
    assert(allocated_ptr == NULL);
    // 11111110000|1111111100
    // 11111110 000|11111 11100...
    assert(bootstrap_allocator_bitmap[0] == 0b11111110);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
    assert(bootstrap_allocator_bitmap[2] == 0b11100111);
    // Alloc 4 pages in DMA
    allocated_ptr = bootstrap_allocator_alloc_pages(4 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
    assert((uintptr_t)allocated_ptr == 7 * 4096);
    // 11111111111|1111111100
    // 11111111 111|11111 11100...
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    assert(bootstrap_allocator_bitmap[2] == 0b11100111);
    // Alloc 2 pages in NORMAL
    allocated_ptr = bootstrap_allocator_alloc_pages(2 * 4096, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
    assert((uintptr_t)allocated_ptr == 19 * 4096);
    // 11111111111|1111111111
    // 11111111 111|11111 11111...
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    assert(bootstrap_allocator_bitmap[2] == 0b11111111);
}
