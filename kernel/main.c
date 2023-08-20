#include "types.h"
#include "proc.h"
#include "uart.h"
#include "printf.h"


volatile u32 setup_complete = 0;

// This is the first function executed in S-Mode.
void main(void) {
  if (this_cpu_id() == 0) {
    uart_init();
    printf_init();

    printf("Hello from cpu #%d\n", this_cpu_id());

    setup_complete = 1;
  } else {
    while (setup_complete == 0) {
      // Spin until system init complete
    }
    printf("Hello from (non-zero) cpu #%d\n", this_cpu_id());
  }

  while (1) {}
}
