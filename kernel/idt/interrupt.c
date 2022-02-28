#include "interrupt.h"
#include "../driver/serialport/serialport.h"
#include "../../capsella_lib/include/test_header.h"
#include "../driver/localapic/timer/l_apic_timer.h"
#include "../gdt/segment.h"
#include <stdint.h>
#include <stdbool.h>

#define END_OF_INTERRUPT *(volatile uint32_t *)(0xfee000b0)

struct InterruptDescriptor IDT[256];

union InterruptDescriptorAttribute MakeIDTAttr(
    enum DescriptorType type,
    uint8_t descriptor_privilege_level) {
    uint8_t interrupt_stack_table = 0;
    bool present = true;
    union InterruptDescriptorAttribute attr;
    attr.bits.interrupt_stack_table = interrupt_stack_table;
    attr.bits.type = type;
    attr.bits.descriptor_privilege_level = descriptor_privilege_level;
    attr.bits.present = present;

    return attr;
}

void SetIDTEntry(struct InterruptDescriptor *desc,
                union InterruptDescriptorAttribute attr,
                uint64_t offset,
                uint16_t segment_descriptor){
    desc->attr = attr;
    desc->offset_low = offset & 0xffffu;
    desc->offset_middle = (offset >> 16) & 0xffffu;
    desc->offset_high = offset >> 32;
    desc->segment_descriptor = segment_descriptor;
}

void NotifyEndOfInterrupt(){
    END_OF_INTERRUPT = 0;
}

__attribute__ ((interrupt))
void IntHandlerLAPICTimer(struct interrupt_frame *frame){
    NotifyEndOfInterrupt();
    test_printk();
}

const uint16_t kKernelCS = 1 << 3;
void InitializeInterrupt(){
    SetIDTEntry(&IDT[0x40],
                MakeIDTAttr(InterruptGate, 0),
                (uint64_t)(IntHandlerLAPICTimer),
                kKernelCS);
    LoadIDT(sizeof(IDT) - 1, (uintptr_t)(&IDT[0]));
}