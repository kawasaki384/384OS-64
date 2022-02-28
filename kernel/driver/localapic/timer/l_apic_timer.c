#include "l_apic_timer.h"
#include "../local_apic.h"
#include <stdint.h>

void InitializeAPICTimer(){
    DIVIDE_CONFIG = 0b1011;
    LVT_TIMER = (0b010 << 16) | 0x40;
    INITIAL_COUNT = COUNT;
}

void StartAPICTimer(){
    INITIAL_COUNT = COUNT;
}

uint32_t LAPICTimerElapsed(){
    return COUNT - CURRENT_COUNT;
}

void StopAPICTimer(){
    INITIAL_COUNT = 0;
}