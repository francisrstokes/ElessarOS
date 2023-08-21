# The trampoline is a mechanism to effectively take traps from U-Mode to S-Mode
# Because risc-v doesn't have any kind of automated swapping of page tables on context switches,
# we need a place that is essentially fixed at the same place in virtual memory in any given user
# process and the kernel.

#include "./inc/memory-map.h"

.section .trampoline
.global trampoline

trampoline:

.align 4
.globl uservec
uservec:
  # Traps from U-Mode end up here. We need to save the context to the process's trap frame.
  # For that, we need a free register. We can store the a0 reg in sscratch, and use a0 as
  # the trap frame base for storing all of the other registers.
  csrw sscratch, a0
  li a0, TRAP_FRAME

  sd ra,  40(a0)
  sd sp,  48(a0)
  sd gp,  56(a0)
  sd tp,  64(a0)
  sd t0,  72(a0)
  sd t1,  80(a0)
  sd t2,  88(a0)
  sd s0,  96(a0)
  sd s1,  104(a0)
  sd a1,  120(a0)
  sd a2,  128(a0)
  sd a3,  136(a0)
  sd a4,  144(a0)
  sd a5,  152(a0)
  sd a6,  160(a0)
  sd a7,  168(a0)
  sd s2,  176(a0)
  sd s3,  184(a0)
  sd s4,  192(a0)
  sd s5,  200(a0)
  sd s6,  208(a0)
  sd s7,  216(a0)
  sd s8,  224(a0)
  sd s9,  232(a0)
  sd s10, 240(a0)
  sd s11, 248(a0)
  sd t3,  256(a0)
  sd t4,  264(a0)
  sd t5,  272(a0)
  sd t6,  280(a0)

  # Read back the original a0 value and write it to the trap frame
  csrr t0, sscratch
  sd t0,  112(a0)

  # The first fields in the trapframe contain context information for the kernel
  ld sp, 8(a0)
  ld tp, 32(a0) // hartid
  ld t0, 16(a0) // address of the actual U-Mode trap handler in C land
  ld t1, 0(a0)  // the kernel page table

  # Force the ordering of previous memory accesses
  sfence.vma zero, zero

  # Swap out for the kernel page table
  csrw satp, t1

  # Force a flush of the TLB cache
  sfence.vma zero, zero

  # Jump into the actual trap handler now that the context has been saved
  jr t0

.global userret
userret:
  # userret is called from C as userret(pt) in order to switch from S-Mode to U-Mode,
  # where pt is a satp-ready pagetable address. Most of this function is essentially the
  # reverse of uservec

  # First switch page tables
  sfence.vma zero, zero
  csrw satp, a0
  sfence.vma zero, zero

  # Get the trap frame offset into a0
  li a0, TRAP_FRAME

  # Restore most of the registers
  ld ra,  40(a0)
  ld sp,  48(a0)
  ld gp,  56(a0)
  ld tp,  64(a0)
  ld t0,  72(a0)
  ld t1,  80(a0)
  ld t2,  88(a0)
  ld s0,  96(a0)
  ld s1,  104(a0)
  ld a1,  120(a0)
  ld a2,  128(a0)
  ld a3,  136(a0)
  ld a4,  144(a0)
  ld a5,  152(a0)
  ld a6,  160(a0)
  ld a7,  168(a0)
  ld s2,  176(a0)
  ld s3,  184(a0)
  ld s4,  192(a0)
  ld s5,  200(a0)
  ld s6,  208(a0)
  ld s7,  216(a0)
  ld s8,  224(a0)
  ld s9,  232(a0)
  ld s10, 240(a0)
  ld s11, 248(a0)
  ld t3,  256(a0)
  ld t4,  264(a0)
  ld t5,  272(a0)
  ld t6,  280(a0)

  # Restore a0
  ld a0,  112(a0)

  # Return to U-Mode
  sret
