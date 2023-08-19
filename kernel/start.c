#include "inc/types.h"
#include "inc/param.h"
#include "inc/asm.h"

__attribute__((aligned(16))) u8 stack0[NCPU * PAGE_SIZE];

void main(void);

void start(void) {
  // Initial setup to get out of M-Mode and into S-Mode

  // Set previous mode to supervisor
  u64 mstatus = r_mstatus();
  mstatus &= ~MSTATUS_MPP_MASK;
  mstatus |= MSTATUS_MPP_S;
  w_mstatus(mstatus);

  // Setup where S-Mode should start execution
  w_mepc((u64)main);

  // Delegate trap handlers to S-Mode
  w_mideleg(0xffff);
  w_medeleg(0xffff);
  w_sie(SIE_SEIE | SIE_SSIE | SIE_STIE);

  // Turn off paging
  w_satp(0);

  // Configure physical memory protection for S-Mode
  w_pmpaddr0(0x1fffffffffffffL); // (1 << 53) - 1, i.e. all possible 53-bit addresses
  w_pmpcfg0(0xf);

  // Place the hartid into the thread pointer register
  u64 hartid = r_mhartid();
  w_tp(hartid);

  // Transfer execution to S-Mode
  asm volatile("mret");
}
