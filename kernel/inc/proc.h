#ifndef INC_PROC_H
#define INC_PROC_H

#include "types.h"

typedef struct cpu_t {
  u32 old_interrupt_state;

  // This is signed so that we can sanity check it falling below zero, which would happen if
  // more pops were observed than pushes had taken place.
  i32 interrupt_disabled_count;
} cpu_t;

cpu_t* this_cpu(void);
u32 this_cpu_id(void);

#endif // INC_PROC_H
