#include "inc/proc.h"
#include "inc/asm.h"
#include "inc/param.h"

static cpu_t cpus[NCPU];

cpu_t* this_cpu(void) {
  u32 hartid = r_tp();
  return &cpus[hartid];
}
