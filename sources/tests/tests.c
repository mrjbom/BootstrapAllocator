#include "tests.h"
#include "../bootstrap_allocator/bootstrap_allocator.h"
#include <assert.h>

void tests_mark_range()
{
    // TEST 1
    // 5 pages, 0x1000 - 0x5000
    bootstrap_allocator_init(1, 5);
    // Mark 5 pages as free
    bootstrap_allocator_mark_range((void*)0x1000, 5 * 4096, 0);
    // Bitmap is 10000011...
    assert(bootstrap_allocator_bitmap[0] == 0b10000011);
    // Mark 6 pages as free(fail)
    bootstrap_allocator_mark_range((void*)0x1000, 6 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000011);
    // Mark 0 pages as free(fail)
    bootstrap_allocator_mark_range((void*)0x1000, 0, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000011);

    // TEST 2
    // 12 pages, 0x1000 - 0xC000

    bootstrap_allocator_init(1, 12);
    // Mark 5 pages as free [0x1000 - 0x5000]
    bootstrap_allocator_mark_range((void*)0x1000, 5 * 4096, 0);
    // Bitmap is 10000011...
    // Mark 3 pages as free [0x6000 - 0x8000]
    bootstrap_allocator_mark_range((void*)0x6000, 3 * 4096, 0);
    // Bitmap is 10000000 01111...
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Fail
    bootstrap_allocator_mark_range((void*)0xC000, 2 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 0xC000 as free
    bootstrap_allocator_mark_range((void*)0xC000, 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01110111);
    // Mark 2 pages [0xB000 - 0xC0000] as used
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 7 pages [0x1000 - 0x7000] as used
    bootstrap_allocator_mark_range((void*)0x1000, 7 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b01111111);
    // Mark 1 page 0x8000 as used
    bootstrap_allocator_mark_range((void*)0x8000, 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 2 pages [0x1000, 0x2000] as free
    bootstrap_allocator_mark_range((void*)0x1000, 2 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10011111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 2 pages [0xB000, 0xC000] as free
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10011111);
    assert(bootstrap_allocator_bitmap[1] == 0b11100111);
    // Mark 3 pages [0x1000, 0x3000] as used
    bootstrap_allocator_mark_range((void*)0x1000, 3 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11100111);
    // Mark 4 pages [0x9000, 0xC000] as free
    bootstrap_allocator_mark_range((void*)0x9000, 4 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b10000111);
    // Mark 12 pages [0x1000, 0xC000] as free
    bootstrap_allocator_mark_range((void*)0x1000, 12 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000000);
    assert(bootstrap_allocator_bitmap[1] == 0b00000111);
    // Mark 2 pages [0x1000, 0x2000] as free
    bootstrap_allocator_mark_range((void*)0x1000, 2 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11100000);
    assert(bootstrap_allocator_bitmap[1] == 0b00000111);
    // Mark 2 pages [0xB000, 0xC000] as free
    bootstrap_allocator_mark_range((void*)0xB000, 2 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11100000);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
    // Mark 8 pages [0x3000, 0xA000] as used
    bootstrap_allocator_mark_range((void*)0x3000, 8 * 4096, 1);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Fail
    bootstrap_allocator_mark_range((void*)0x0, 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Fail
    bootstrap_allocator_mark_range((void*)0x0, 13 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Fail
    bootstrap_allocator_mark_range((void*)0xD000, 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b11111111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 4 pages [0x1000, 0x4000] as free
    bootstrap_allocator_mark_range((void*)0x1FFF, 4 * 4096, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000111);
    assert(bootstrap_allocator_bitmap[1] == 0b11111111);
    // Mark 5 pages [0x6000, 0xA000] as free
    bootstrap_allocator_mark_range((void*)0x6000, 4 * 4096 + 64, 0);
    assert(bootstrap_allocator_bitmap[0] == 0b10000100);
    assert(bootstrap_allocator_bitmap[1] == 0b00011111);
}
