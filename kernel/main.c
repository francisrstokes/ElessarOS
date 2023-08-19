#include "inc/types.h"
#include "inc/proc.h"
#include "inc/uart.h"

char* cpu0_message = "Hello from CPU #0!\n";
char* cpuN_message = "Hello from CPU >0!\n";

volatile u32 setup_complete = 0;

// This is the first function executed in S-Mode.
void main(void) {
  if (this_cpu_id() == 0) {
    uart_init();

    uart_write_str(cpu0_message);

    setup_complete = 1;
  } else {
    while (setup_complete == 0) {
      // Spin until system init complete
    }
    uart_write_str(cpuN_message);
  }

  while (1) {}
}
