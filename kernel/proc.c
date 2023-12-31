#include "proc.h"
#include "asm.h"
#include "param.h"

static cpu_t cpus[NCPU];

cpu_t* this_cpu(void) {
  u32 hartid = r_tp();
  return &cpus[hartid];
}

u32 this_cpu_id(void) {
  return r_tp();
}
