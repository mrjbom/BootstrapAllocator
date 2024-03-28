# BootstrapAllocator

A simple and primitive allocator that uses a bitmap to pages allocation.  
It is focused on working in the OS kernel to initialize the memory required to configure the main allocator and other things (page tables).  
The bitmap memory is reserved in advance for 4 GB of memory.  
It is focused on use in a 32-bit address space.  

The allocator is designed for x86, so there is a division into DMA and NORMAL zones.  
The DMA zone is usually located in the first 16 megabytes, and the NORMAL zone after it.  

## How to use
```
// 10 pages DMA: 0x1000 - 0xA000
// 10 pages NORMAL: 0xB000 - 0x14000
// Zero page is reserved!
bootstrap_allocator_init(1, 10, 11, 20);

// Mark all pages as FREE
bootstrap_allocator_mark_range((void*)0x1000, 20 * 4096, BOOTSTRAP_ALLOCATOR_STATE_FREE);

// Alloc 1 page from DMA zone
void* allocated_dma_ptr = bootstrap_allocator_alloc_pages(1, BOOTSTRAP_ALLOCATOR_ZONE_DMA);
// allocated_dma_ptr == 0x1000

// Alloc 1 page from NORMAL zone
void* allocated_normal_ptr = bootstrap_allocator_alloc_pages(1, BOOTSTRAP_ALLOCATOR_ZONE_NORMAL);
// allocated_normal_ptr == 0xB000
```