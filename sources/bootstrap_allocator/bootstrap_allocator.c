#include "bootstrap_allocator.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define GET_BIT(num, bit) (((num) >> (bit)) & 1)
#define SET_BIT(num, bit) ((num) |= (1 << (bit)))
#define CLEAR_BIT(num, bit) ((num) &= ~(1 << (bit)))
#define GET_BIT_MSB(num, bit) (((num) >> ((sizeof(num) * 8) - 1 - (bit))) & 1)
#define SET_BIT_MSB(num, bit) ((num) |= (1 << ((sizeof(num) * 8) - 1 - (bit))))
#define CLEAR_BIT_MSB(num, bit) ((num) &= ~(1 << ((sizeof(num) * 8) - 1 - (bit))))

size_t bootstrap_allocator_min_pfn = 0;
size_t bootstrap_allocator_max_pfn = 0;
uint8_t bootstrap_allocator_bitmap[BOOTSTRAP_ALLOCATOR_BITMAP_SIZE];

void bootstrap_allocator_init(uint32_t min_pfn, uint32_t max_pfn)
{
    if (min_pfn == 0 || min_pfn > max_pfn || max_pfn > BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER) {
        return;
    }
    bootstrap_allocator_min_pfn = min_pfn;
    bootstrap_allocator_max_pfn = max_pfn;
    // Mark all pages as used
    memset(bootstrap_allocator_bitmap, UINT8_MAX, BOOTSTRAP_ALLOCATOR_BITMAP_SIZE);
}

void bootstrap_allocator_mark_range(void* addr, size_t size, bool state)
{
    if ((uintptr_t)addr < BOOTSTRAP_ALLOCATOR_PAGE_SIZE || size < BOOTSTRAP_ALLOCATOR_PAGE_SIZE) {
        return;
    }
    size_t current_page_index = (uintptr_t)addr / BOOTSTRAP_ALLOCATOR_PAGE_SIZE;
    if (current_page_index < bootstrap_allocator_min_pfn) {
        return;
    }
    size_t number_of_pages = size / BOOTSTRAP_ALLOCATOR_PAGE_SIZE;
    if (size % BOOTSTRAP_ALLOCATOR_PAGE_SIZE != 0) {
        number_of_pages++;
    }
    if (current_page_index + number_of_pages - 1 > bootstrap_allocator_max_pfn) {
        return;
    }

    // Iterate over the pages and mark them accordingly
    for (size_t i = 0; i < number_of_pages; i++) {
        // Calculate the byte and bit offset within the bitmap
        size_t byte_offset = current_page_index / 8;
        size_t bit_offset = current_page_index % 8;

        // Set or clear the bit based on the state
        if (state) {
            SET_BIT_MSB(bootstrap_allocator_bitmap[byte_offset], bit_offset);
        }
        else {
            CLEAR_BIT_MSB(bootstrap_allocator_bitmap[byte_offset], bit_offset);
        }
        current_page_index++;
    }
}

void* bootstrap_allocator_alloc_pages(size_t size)
{
    return NULL;
}
