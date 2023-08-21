#ifndef INC_MEMORY_MAP_H
#define INC_MEMORY_MAP_H

#include "param.h"

#define UART_BASE             (0x10000000)

#define RAM_BASE              (0x80000000)
#define RAM_SIZE              (128 * 1024 * 1024) // 128MiB
#define PHSYICAL_TOP          (RAM_BASE + RAM_SIZE)

#define PAGE_ROUNDUP(addr)    (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUNDDOWN(addr)  ((addr) & ~(PAGE_SIZE - 1))

// 3x 9-bit page table index + 1x 12-bit page offset
#define MAX_VIRT_ADDR (1L << (9 + 9 + 9 + 12 - 1))
#define TRAMPOLINE    (MAX_VIRT_ADDR - PAGE_SIZE)
#define TRAP_FRAME    (TRAMPOLINE - PAGE_SIZE)

#endif // INC_MEMORY_MAP_H
