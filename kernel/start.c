#include "types.h"
#include "param.h"
#include "asm.h"
#include "memory-map.h"

typedef struct timer_scratch_t {
  u64 scratch[3];
  u64 mtimecmp_addr;
  u64 interval;
} timer_scratch_t;

__attribute__((aligned(16))) u8 stack0[NCPU * PAGE_SIZE];
timer_scratch_t timer_scratch[NCPU];

extern void mmode_timer_vec(void);
void main(void);
void mmode_timer_init(void);

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
  w_sie(r_sie() | SIE_SEIE | SIE_SSIE | SIE_STIE);

  // Turn off paging
  w_satp(0);

  // Configure physical memory protection for S-Mode
  w_pmpaddr0(0x1fffffffffffffL); // (1 << 53) - 1, i.e. all possible 53-bit addresses
  w_pmpcfg0(0xf);

  // Setup M-Mode timer interrupts
  mmode_timer_init();

  // Place the hartid into the thread pointer register
  u64 hartid = r_mhartid();
  w_tp(hartid);

  // Transfer execution to S-Mode
  asm volatile("mret");
}

void mmode_timer_init(void) {
  // Using the CLINT (core local interrupt controller) memory mapped peripheral, we can set up
  // to receive M-Mode timer interrupts at a fixed frequency. These interrupts can be passed on
  // to S-Mode by setting the software interrupt pending bit in the sip CSR.
  // The M-Mode handler is implemented in timervec.S
  u64 hartid = r_mhartid();

  // Set up to receive the first interrupt
  const u64 interval = 1000000; // cycles; about 1/10th second in qemu.
  *(u64*)CLINT_MTIMECMP(hartid) = *(u64*)CLINT_MTIME + interval;

  // Setup the scratch area which will be used by each harts interrupt handler to schedule
  // the next interrupt
  timer_scratch[hartid].mtimecmp_addr = CLINT_MTIMECMP(hartid);
  timer_scratch[hartid].interval = interval;
  w_mscratch((u64)&timer_scratch[hartid]);

  // Install the interrupt vector handler, and enable machine timer interrupts
  w_mtvec((u64)mmode_timer_vec);
  w_mstatus(r_mstatus() | MSTATUS_MIE);
  w_mie(r_mie() | MIE_MTIE);
}
