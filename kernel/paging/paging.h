#ifndef PAGING_H_
#define PAGING_H_

#include <stdint.h>
#include <stddef.h>
void SetCR3(uint64_t value);
void SetupIdentityPagetable();
//static const size_t page_directory_count = 64;
#endif /*PAGING_H_*/
