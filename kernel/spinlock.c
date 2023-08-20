#include "spinlock.h"
#include "proc.h"
#include "interrupts.h"
#include "printf.h"

void acquire_push_interrupt_state(void) {
  u32 old_interrupt_state = interrupts_get();
  interrupts_off();

  cpu_t* cpu = this_cpu();
  if (cpu->interrupt_disabled_count == 0) {
    cpu->old_interrupt_state = old_interrupt_state;
  }
  cpu->interrupt_disabled_count++;
}

void release_pop_interrupt_state(void) {
  if (interrupts_get() != 0) {
    panic("pop int");
  }

  cpu_t* cpu = this_cpu();

  cpu->interrupt_disabled_count--;
  if (cpu->interrupt_disabled_count < 0) {
    panic("pop count");
  }

  if (cpu->interrupt_disabled_count == 0 && cpu->old_interrupt_state) {
    interrupts_on();
  }
}

void spinlock_init(spinlock_t* lk, char* name) {
  lk->locked = 0;
  lk->name = name;
}

void spinlock_acquire(spinlock_t* lk) {
  acquire_push_interrupt_state();

  if (this_cpu() == lk->cpu) {
    panic("acquire");
  }

  while (__sync_lock_test_and_set(&lk->locked, 1) != 0) {
    // Spin until we've acquired the lock
  }

  // Memory barrier to ensure that subsequent operations are not reordered
  __sync_synchronize(); // FENCE

  lk->cpu = this_cpu();
}

void spinlock_release(spinlock_t* lk) {
  if (this_cpu() != lk->cpu) {
    panic("release");
  }

  lk->cpu = NULL;

  // Memory barrier to ensure that subsequent operations are not reordered
  __sync_synchronize(); // FENCE
  __sync_lock_release(&lk->locked);

  release_pop_interrupt_state();
}
