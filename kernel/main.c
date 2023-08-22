#include "types.h"
#include "proc.h"
#include "uart.h"
#include "printf.h"
#include "kmem.h"
#include "vm.h"

volatile u32 setup_complete = 0;

void print_logo(void);

// This is the first function executed in S-Mode.
void main(void) {
  if (this_cpu_id() == 0) {
    uart_init();
    printf_init();

    print_logo();

    kmem_init();
    vm_init();
    vm_hart_init();
    printf("[vm] Page table setup complete\n");

    setup_complete = 1;
  } else {
    while (setup_complete == 0) {
      // Spin until system init complete
    }

    vm_hart_init();
    printf("Hart #%d online\n", this_cpu_id());
  }

  while (1) {}
}

void print_logo(void) {
  printf(
    ".---..                         .--.  .-. \n"
    "|    |                        :    :(   )\n"
    "|--- |  .-. .--..--. .-.  .--.|    | `-. \n"
    "|    | (.-' `--.`--.(   ) |   :    ;(   )\n"
    "'---'`- `--'`--'`--' `-'`-'    `--'  `-' \n"
  );
}
