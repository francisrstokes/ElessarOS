#include "inc/types.h"
#include "inc/asm.h"

// This is the first function executed in S-Mode.
void main(void) {
  if (r_tp() == 0) {
    // Setup uart

    while (1) {}
  } else {
    while (1) {}
  }
}
