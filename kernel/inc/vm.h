#ifndef INC_VM_H
#define INC_VM_H

#include "types.h"
#include "param.h"

#define VM_PAGE_INDEX_MASK         ((1 << 9) - 1)
#define VM_PAGE_INDEX_SHIFT(level) (PAGE_BITS + (9 * level))
#define VM_PAGE_INDEX(va, level)   ((((u64)(va)) >> VM_PAGE_INDEX_SHIFT(level)) & VM_PAGE_INDEX_MASK)

#define VM_PTE2PA(pte)  ((pte >> 10) << PAGE_BITS)
#define VM_PA2PTE(pa)   ((pa >> PAGE_BITS) << 10)

#define VM_PT2SATP(pt)  ((8L << 60) | (pt >> 12))

#define PTE_V (1L << 0)
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)

typedef u64 pte_t;
typedef u64* pagetable_t; // PAGE_SIZE / sizeof(u64) = 512 PTEs

void vm_init(void);
void vm_hart_init(void);

#endif // INC_VM_H
