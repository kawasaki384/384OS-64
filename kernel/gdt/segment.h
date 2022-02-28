#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>

enum DescriptorType
{
    Upper8bites = 0,
    LDT = 2,
    TTSAvailable = 9,
    TSSBusy = 11,
    CallGate = 12,
    InterruptGate = 14,
    Trapgate = 15,

    ReadWrite = 2,
    ExecuteRead = 10,
};

union SegmentDiscriptor{
    uint64_t data;
    struct {
        uint64_t limit0 : 16;
        uint64_t base0 : 16;
        uint64_t base1 : 8;
        enum DescriptorType type :4;
        uint64_t system_segment: 1;
        uint64_t descriptor_privilege_level :2;
        uint64_t present : 1;
        uint64_t limit1 : 4;
        uint64_t available : 1;
        uint64_t long_mode : 1;
        uint64_t default_operation_size: 1;
        uint64_t granularity : 1;
        uint64_t base2 : 8;
    } __attribute__((packed)) bits;
} __attribute__((packed));

extern union SegmentDiscriptor gdt[3];

#endif /*SEGMENT_H*/
