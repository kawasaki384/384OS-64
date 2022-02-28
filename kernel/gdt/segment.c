#include "segment.h"
#include "gdt_func.h"

//ゼロからのOS自作入門p191など。

union SegmentDiscriptor gdt[3];
void SetCodeSegment(union SegmentDiscriptor *desc,
                    enum DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit)
{
    desc->data = 0;

    desc->bits.base0 = base & 0xffffu;
    desc->bits.base1 = (base >> 16) & 0xffu;
    desc->bits.base2 = (base >> 24) & 0xffu;

    desc->bits.limit0 = limit & 0xffffu;
    desc->bits.limit1 = (limit >> 16) & 0xfu;

    desc->bits.type = type;

    desc->bits.system_segment = 1; //Code_Data_Segment
    desc->bits.descriptor_privilege_level = descriptor_privilege_level;
    desc->bits.present = 1;
    desc->bits.available = 0;
    desc->bits.long_mode = 1;
    desc->bits.default_operation_size = 0; //should be 0 when long_mode == 1
    desc->bits.granularity = 1;
}

void SetDataSegment(union SegmentDiscriptor *desc,
                    enum DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit)
{
    SetCodeSegment(desc, type, descriptor_privilege_level, base, limit);
    desc->bits.long_mode = 0;
    desc->bits.default_operation_size = 1; //32bit stack segment
}

void SetupSegments() {
    gdt[0].data = 0;
    SetCodeSegment(&gdt[1], ExecuteRead, 0, 0, 0xfffff);
    SetDataSegment(&gdt[2], ReadWrite, 0, 0, 0xfffff);
    LoadGDT(sizeof(gdt) - 1, (uintptr_t)(&gdt[0]));
}
