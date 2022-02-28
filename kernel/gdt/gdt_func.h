#ifndef GDT_FUNC_H

#define GDT_FUNC_H
#include <stdint.h>
#include "segment.h"
void LoadGDT(uint16_t limit, uint64_t offset);
void SetDSAll(uint16_t value);
void SetCSSS(uint16_t cs, uint16_t ss);
void SetupSegments();
#endif /*GDT_FUNC_H*/