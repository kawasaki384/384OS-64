#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "memmgr.h"

#include "../args/memmap.h"
#include "../args/memory.h"
#include "../driver/serialport/serialport.h"

#define UEFIPAGESIZE 4096

struct BitMapMemoryManager BMM = {{},0,128*1024/4};

bool GetBit(size_t frame_id) {
    size_t line_index = frame_id / BitsPerMapLine;
    size_t bit_index = frame_id % BitsPerMapLine;

    return (BMM.alloc_map_[line_index] & (unsigned long)(1) << bit_index) != 0;
}

void SetBit(size_t frame_id,bool allocated){
    size_t line_index = frame_id / BitsPerMapLine;
    size_t bit_index = frame_id % BitsPerMapLine;

    if(allocated){
        BMM.alloc_map_[line_index] |= (unsigned long)(1) << bit_index;
    } else {
        BMM.alloc_map_[line_index] &= ~(unsigned long)(1) << bit_index;
    }
}

extern caddr_t program_break,program_break_end;

void MarkAllocated(size_t start_frame_id,size_t num_frames){
    for (size_t i = 0; i < num_frames; ++i){
        SetBit(start_frame_id + i, true);
    }
}

int Free(size_t start_frame_id,size_t num_frames){
    for (size_t i = 0; i < num_frames;++i){
        SetBit(start_frame_id + i, false);
    }
    return 1;
}

void SetMemoryRange(size_t range_begin_id,size_t range_end_id){
    BMM.range_begin_id = range_begin_id;
    BMM.range_end_id = range_end_id;
}

int Allocate(size_t num_frames){
    size_t start_frame_id = BMM.range_begin_id;
    while(1){
        size_t i = 0;
        for(; i < num_frames; ++i){
            if(start_frame_id + i >= BMM.range_end_id){
                return 1;
            }
            if(GetBit(start_frame_id + i)){
                break;
            }
        }
        if(i == num_frames){
            MarkAllocated(start_frame_id, num_frames);
            return 1;
        };
        start_frame_id += i + 1;
    }
}

int InitializeHeap(){
    const int HeapFrames = 64*512;
    const size_t heap_start = Allocate(HeapFrames);
    if(heap_start == 0){
        return 0;
    }
    program_break = (caddr_t)(heap_start * BytesPerFrame);
    program_break_end = program_break + HeapFrames * BytesPerFrame;
    return 1;
}

int InitializeMemoryManager(MemoryMap *memorymap){
    const uintptr_t memory_map_base = (uintptr_t)(memorymap->buffer);
    uintptr_t aviailable_end = 0;
    for (uintptr_t iter = memory_map_base;
         iter < memory_map_base + memorymap->map_size;
         iter += memorymap->descriptor_size){
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)iter;
        uintptr_t Physicalstart = desc->PhysicalStart;
        if(aviailable_end < (uintptr_t)desc->PhysicalStart){
            MarkAllocated(aviailable_end / BytesPerFrame,
                          (Physicalstart - aviailable_end) / BytesPerFrame);
        }

        const uintptr_t physical_end = Physicalstart + desc->NumberOfPages * UEFIPAGESIZE;
        if(desc->Type == EfiBootServicesCode || desc->Type == EfiBootServicesData
                        || desc->Type == EfiConvertionalMemory){
            aviailable_end = physical_end;
        }else{
            MarkAllocated(desc->PhysicalStart / BytesPerFrame,
                          desc->NumberOfPages * UEFIPAGESIZE / BytesPerFrame);
        }
    }
    SetMemoryRange(1, aviailable_end / BytesPerFrame);
    return InitializeHeap();
}
