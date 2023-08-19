#ifndef INC_SPINLOCK_H
#define INC_SPINLOCK_H

#include "inc/types.h"

typedef struct spinlock_t {
  u32 locked;

  char* name;
  struct cpu_t* cpu;
} spinlock_t;

// These functions are mostly used by spinlocks, but are occasionally piggybacked
// by other parts of the kernel too
void acquire_push_interrupt_state(void);
void release_pop_interrupt_state(void);

void spinlock_init(spinlock_t* lk, char* name);
void spinlock_acquire(spinlock_t* lk);
void spinlock_release(spinlock_t* lk);

#endif // INC_SPINLOCK_H
