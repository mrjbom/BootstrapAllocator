# BootstrapAllocator

A simple and primitive allocator that uses a bitmap to pages allocation.  
It is focused on working in the OS kernel to initialize the memory required to configure the main allocator and other things (page tables).  
The bitmap memory is reserved in advance for 4 GB of memory.  
It is focused on use in a 32-bit address space.  