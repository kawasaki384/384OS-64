#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "../gdt/segment.h"

void LoadIDT(uint16_t limit, uint64_t offset);

union InterruptDescriptorAttribute{
    uint16_t data;
    struct {
        uint16_t interrupt_stack_table : 3;
        uint16_t : 5;
        enum DescriptorType type : 4;
        uint16_t : 1;
        uint16_t descriptor_privilege_level : 2;
        uint16_t present : 1;
    } __attribute((packed)) bits;
}__attribute__((packed));

struct InterruptDescriptor {
    uint16_t offset_low;
    uint16_t segment_descriptor;
    union InterruptDescriptorAttribute attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct interrupt_frame{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void InitializeInterrupt();

#endif /*INTERRUPT_H_*/