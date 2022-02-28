#ifndef L_APIC_TIMER_H_
#define L_APIC_TIMER_H_

#define COUNT 0xffffffffu
void test_printk();

#include <stdint.h>
void InitializeAPICTimer();
void StartAPICTimer();
uint32_t LAPICTimerElapsed();
void StopAPICTimer();

#endif /*L_APIC_TIMER_H_*/