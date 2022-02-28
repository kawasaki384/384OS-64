#ifndef LOCAL_APIC_H_
#define LOCAL_APIC_H_

#include <stdint.h>
/*
ISR_0_31:In-Service Register(ISR) (bits 0:31)
ISR_31_63:In-Service Register(ISR) (bits 32:63)
みたいな感じ。
*/

/*LocalAPICTable*/
#define LAPIC_BASE 0xfee00000

#define LOCAL_APIC_ID *(volatile uint32_t*)(0xfee00020)
#define LOCAL_APIC_VER *(volatile uint32_t*)(0xfee00030)
#define TASK_POLICY *(volatile uint32_t*)(0xfee00080)
#define ARBITRATION_PRIORITY *(volatile uint32_t*)(0xfee00090) //xAPIC-mode-only
#define PROCESSOR_PRIORITY *(volatile uint32_t *)(0xfee000a0)
#define EOI *(volatile uint32_t *)(0xfee000b0)
#define REMOTE_READ *(volatile uint32_t*)(0xfee000c0) //xAPIC-mode-only
#define LOGICAL_DESTINATION *(volatile uint32_t*)(0xfee000d0)
#define DESTINATION_FORMAT *(volatile uint32_t*)(0xfee000e0) //xAPIC-mode-only
#define SPURIOUS_INTERRPUT_VECTOR *(volatile uint32_t *)(0xfee000f0)

/*In-Service Register(ISR)*/

#define ISR_0_31 *(volatile uint32_t *)(0xfee00100) /*0xfee0100 = APIC_BASE + 100*/
//もしxAPICModeの場合,ISR_31_0になる。
#define ISR_32_63 *(volatile uint32_t*)(0xfee00110)
#define ISR_64_95 *(volatile uint32_t*)(0xfee00120)
#define ISR_96_127 *(volatile uint32_t*)(0xfee00130)
#define ISR_128_159 *(volatile uint32_t*)(0xfee00140)
#define ISR_160_191 *(volatile uint32_t*)(0xfee00150)
#define ISR_192_223 *(volatile uint32_t*)(0xfee00160)
#define ISR_224_255 *(volatile uint32_t*)(0xfee00170)

/*In-Service_Register(ISR)*/

/*Trigger Mode Register(TMR)*/

#define TMR_0_31 *(volatile uint32_t*)(0xfee00180)
#define TMR_32_63 *(volatile uint32_t*)(0xfee00190)
#define TMR_64_95 *(volatile uint32_t*)(0xfee001A0)
#define TMR_96_127 *(volatile uint32_t*)(0xfee001B0)
#define TMR_128_159 *(volatile uint32_t*)(0xfee001C0)
#define TMR_160_191 *(volatile uint32_t*)(0xfee001D0)
#define TMR_192_223 *(volatile uint32_t*)(0xfee001E0)
#define TMR_224_255 *(volatile uint32_t*)(0xfee001F0)

/*Trigger Mode Register(TMR)*/

/*Interrupt Request Register(IRR)*/
#define IRR_0_31 *(volatile uint32_t*)(0xfee00200)
#define IRR_32_63 *(volatile uint32_t*)(0xfee00210)
#define IRR_64_95 *(volatile uint32_t*)(0xfee00220)
#define IRR_96_127 *(volatile uint32_t*)(0xfee00230)
#define IRR_128_159 *(volatile uint32_t*)(0xfee00240)
#define IRR_160_191 *(volatile uint32_t*)(0xfee00250)
#define IRR_192_223 *(volatile uint32_t*)(0xfee00260)
#define IRR_224_255 *(volatile uint32_t*)(0xfee00270)
/*Interrupt Request Register(IRR)*/

#define ERROR_STATUS *(volatile uint32_t*)(0xfee00280)

/*LVT Corrected Machine Check Interrupt Register (CMCI)*/
#define CMCI *(volatile uint32_t*)(0xfee002f0) //xAPIC-mode-only
/*LVT Corrected Machine Check Interrupt Register (CMCI)*/

/*Interrput Command Register(ICR)*/
#define ICR_0_31 *(volatile uint32_t*)(0xfee00300)
#define ICR_32_63 *(volatile uint32_t*)(0xfee00310)
/*Interrput Command Register(ICR)*/

/*Local Vector Table Register (LVT)*/
#define LVT_TIMER *(volatile uint32_t *)(0xfee00320)
#define LVT_THERNAL_SENSOR *(volatile uint32_t*)(0xfee00330)
#define LVT_PERFORMANCE_COUNTER *(volatile uint32_t*)(0xfee00340)
#define LVT_LINT0 *(volatile uint32_t*)(0xfee00350)
#define LVT_LINT1 *(volatile uint32_t*)(0xfee00360)
#define LVT_ERROR *(volatile uint32_t*)(0xfee00370)
/*Local Vector Table Register (LVT)*/

#define INITIAL_COUNT *(volatile uint32_t *)(0xfee00380)
#define CURRENT_COUNT *(volatile uint32_t *)(0xfee00390)
#define DIVIDE_CONFIG *(volatile uint32_t *)(0xfee003e0)

#endif /*LOCAL_APIC_H_*/

/*LocalAPICTable*/
