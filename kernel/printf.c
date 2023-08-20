#include "printf.h"
#include "spinlock.h"
#include "uart.h"

typedef struct printf_state_t {
  spinlock_t lock;
  u32 use_lock;
} printf_state_t;

static printf_state_t printf_state;

// Global that can be referenced in other parts of the system. xv6 puts it here because panic sets it,
// which makes some sense.
volatile int panicked = 0;

static const char* digits = "0123456789abcdef";

static void print_int(i32 value, i32 base, i32 sign) {
  char buf[10] = {0};
  i32 x = 0;
  i32 i = 0;

  if(sign && (sign = value < 0))
    x = -value;
  else
    x = value;

  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0) {
    uart_write((i32)buf[i]);
  }
}

void print_ptr(u64 ptr) {
  uart_write((i32)'0');
  uart_write((i32)'x');

  u32 nibble;
  for (int i = 0; i < (sizeof(u64) * 2); i++) {
    nibble = (ptr >> (60 - i * 4)) & 0xf;
    uart_write((i32)digits[nibble]);
  }
}

void printf_init(void) {
  spinlock_init(&printf_state.lock, "printf");
  printf_state.use_lock = 1;
}

void printf(char *fmt, ...) {
  va_list ap;
  char c;

  if (printf_state.use_lock) {
    spinlock_acquire(&printf_state.lock);
  }

  va_start(ap, fmt);
  while (*fmt != 0) {
    c = *fmt;
    fmt++;

    // If this isn't a format string, then just print it
    if (c != '%') {
      uart_write((i32)c);
      continue;
    }

    // Otherwise, figure out how to format the argument
    c = *fmt;
    fmt++;

    switch (c) {
      case 'd': {
        print_int(va_arg(ap, int), 10, 1);
      } break;

      case 'x': {
        print_int(va_arg(ap, int), 16, 0);
      } break;

      case 'p': {
        print_ptr(va_arg(ap, u64));
      } break;

      case '%': {
        uart_write((i32)'%');
      } break;

      default: {
        uart_write((i32)'%');
        uart_write((i32)c);
      } break;
    }
  }
  va_end(ap);

  if (printf_state.use_lock) {
    spinlock_release(&printf_state.lock);
  }
}

void panic(char* str) {
  printf_state.use_lock = 0;
  printf("panic: ");
  printf(str);
  printf("\n");
  panicked = 1;

  while (1) {
    // spin forever
  }
}
