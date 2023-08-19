#ifndef INC_SPINLOCK_H
#define INC_SPINLOCK_H

#include "inc/types.h"

typedef struct spinlock_t {
  u32 locked;

  char* name;
  struct cpu_t* cpu;
} spinlock_t;

void spinlock_init(spinlock_t* lk, char* name);
void spinlock_acquire(spinlock_t* lk);
void spinlock_release(spinlock_t* lk);

#endif // INC_SPINLOCK_H
