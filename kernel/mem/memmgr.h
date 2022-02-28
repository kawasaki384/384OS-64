#ifndef MEM_MGR_H
#define MEM_MGR_H

#include <stdbool.h>
#include <stdint.h>
#include "../args/memmap.h"

#define BytesPerFrame 4096
#define BitsMapLine sizeof(unigned long)
#define BitsPerMapLine 32
struct BitMapMemoryManager
{
    unsigned long alloc_map_[128 * 1024 / 4];
    size_t range_begin_id;
    size_t range_end_id;
};

int InitializeMemoryManager(MemoryMap *memorymap);

#endif /*MEM_MGR_H*/