#include "vm.h"
#include "kmem.h"
#include "kstring.h"
#include "param.h"
#include "printf.h"
#include "asm.h"
#include "memory-map.h"

extern char etext[];      // kernel.ld sets this to end of kernel code.
extern char trampoline[]; // trampoline.S

static pagetable_t kernel_pagetable;

static pagetable_t vm_make_kernel_pagetable(void);
static void vm_map_kernel_pages(pagetable_t pt, u64 va, u64 pa, u64 size, u64 permissions);

void vm_init(void) {
  kernel_pagetable = vm_make_kernel_pagetable();
}

void vm_hart_init(void) {
  sfence_vma();
  w_satp(VM_PT2SATP((u64)kernel_pagetable));
  sfence_vma();
}

static pagetable_t vm_make_kernel_pagetable(void) {
  pagetable_t pt;

  // Allocate a page for the root of the kernel pagetable
  pt = (pagetable_t)kalloc();
  memset(pt, 0, PAGE_SIZE);

  // Create direct map (pa == va) page table entries (PTEs) for various parts of the system
  // UART
  vm_map_kernel_pages(pt, UART_BASE, UART_BASE, PAGE_SIZE, PTE_R | PTE_W);

  // Kernel code + data
  vm_map_kernel_pages(pt, KERNEL_BASE, KERNEL_BASE, (u64)etext - KERNEL_BASE, PTE_R | PTE_X);
  vm_map_kernel_pages(pt, (u64)etext, (u64)etext, (u64)PHYSICAL_TOP - (u64)etext, PTE_R | PTE_W);

  // The trampoline is *not* 1-to-1, but instead is mapped up at the highest possible virtual address
  vm_map_kernel_pages(pt, TRAMPOLINE, (u64)trampoline, PAGE_SIZE, PTE_R | PTE_X);

  return pt;
}

static pte_t* vm_walk(pagetable_t pt, u64 va, i32 alloc) {
  if (va > MAX_VIRT_ADDR) {
    panic("vm_walk");
  }

  for (i32 level = 2; level > 0; level--) {
    pte_t* pte = &pt[VM_PAGE_INDEX(va, level)];

    // Is this page table entry valid?
    if (*pte & PTE_V) {
      // Then we can use the physical address it points to as the next page table root
      pt = (pagetable_t)VM_PTE2PA(*pte);
    } else {
      // In case it's not, should *we* allocate a page for this part of the tree?
      if (alloc != 0) {
        pt = (pte_t*)kalloc();
        if (pt == NULL) {
          return NULL;
        }

        // Clear the page, and mark the root as valid
        memset(pt, 0, PAGE_SIZE);
        *pte = VM_PA2PTE((u64)pt) | PTE_V;
      } else {
        // If the page is invalid, and we're not going to allocate a level, just return null
        return NULL;
      }
    }
  }

  return &pt[VM_PAGE_INDEX(va, 0)];
}

static i32 vm_map_pages(pagetable_t pt, u64 va, u64 pa, u64 size, u64 permissions) {
  if (size == 0) {
    panic("vm_map_pages size");
  }

  // The very first virtual address may not lie on a page boundary, so we round down
  u64 current_va = PAGE_ROUNDDOWN(va);

  // Likewise, the last virtual address doesn't need to be page aligned, so round up
  u64 last_va = PAGE_ROUNDUP(va + size - 1);

  pte_t* pte;
  do {
    // Get the leaf page table entry associated with this address, allocating along the way as needed
    pte = vm_walk(pt, current_va, 1);
    if (pte == NULL) {
      volatile int x = 0;
      x++;
      return -1;
    }

    // If this page is somehow already mapped, that would not make sense.
    if (*pte & PTE_V) {
      panic("vm_map_ages valid");
    }

    // Setup the PTE with the provided permissions
    *pte = VM_PA2PTE(pa) | permissions | PTE_V;

    current_va += PAGE_SIZE;
    pa += PAGE_SIZE;
  } while (current_va != last_va);

  return 0;
}

static void vm_map_kernel_pages(pagetable_t pt, u64 va, u64 pa, u64 size, u64 permissions) {
  if (vm_map_pages(pt, va, pa, size, permissions) != 0) {
    panic("vm_map_kernel_pages");
  }
}