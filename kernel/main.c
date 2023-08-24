#include "types.h"
#include "proc.h"
#include "uart.h"
#include "printf.h"
#include "kmem.h"
#include "vm.h"
#include "asm.h"

volatile u32 setup_complete = 0;

void print_logo(void);
void kernel_vec(void);

void temp_vec(void) {
  printf("[timer] %d\n", this_cpu_id());
  w_sip(r_sip() & ~(SIP_SSIP));
}

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

  w_stvec((u64)kernel_vec);
  w_sstatus(r_sstatus() | SSTATUS_SIE);

  while(1) {

  }
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
