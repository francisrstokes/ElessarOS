#ifndef INC_MEMORY_MAP_H
#define INC_MEMORY_MAP_H

#include "param.h"

#define UART_BASE               (0x10000000L)

#define CLINT_BASE              (0x02000000L)
#define CLINT_MTIMECMP(hartid)   (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME              (CLINT_BASE + 0xBFF8) // cycles since boot.

#define RAM_BASE                 (0x80000000L)
#define RAM_SIZE                 (128L * 1024L * 1024L) // 128MiB
#define PHYSICAL_TOP             (RAM_BASE + RAM_SIZE)

#define KERNEL_BASE              (RAM_BASE)

#define PAGE_ROUNDUP(addr)       (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUNDDOWN(addr)     ((addr) & ~(PAGE_SIZE - 1))

// 3x 9-bit page table index + 1x 12-bit page offset
#define MAX_VIRT_ADDR (1L << (9 + 9 + 9 + 12 - 1))
#define TRAMPOLINE    (MAX_VIRT_ADDR - PAGE_SIZE)
#define TRAP_FRAME    (TRAMPOLINE - PAGE_SIZE)

#endif // INC_MEMORY_MAP_H
