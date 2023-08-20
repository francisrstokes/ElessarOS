#include "interrupts.h"
#include "asm.h"

u32 interrupts_get(void) {
  u64 sstatus = r_sstatus();
  return (sstatus >> SSTATUS_SIE) & 1;
}

void interrupts_off(void) {
  w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

void interrupts_on(void) {
  w_sstatus(r_sstatus() | SSTATUS_SIE);
}
