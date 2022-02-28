#ifndef EFI_ARGS_MEMMAP_H_
#define EFI_ARGS_MEMMAP_H_

#define UINTN uint64_t

typedef struct MEMORYMAP
{
    UINTN buffer_size;
    void *buffer;
    UINTN map_size;
    UINTN map_key;
    UINTN descriptor_size;
    uint32_t descriptor_version;
} MemoryMap;

#endif /*EFI_ARGS_MEMMAP_H_*/