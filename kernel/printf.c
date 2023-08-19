#include "inc/printf.h"
#include "inc/spinlock.h"
#include "inc/uart.h"

spinlock_t printf_lock;

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

void printf_init(void) {
  spinlock_init(&printf_lock, "printf");
}

void printf(char *fmt, ...) {
  va_list ap;
  char c;

  spinlock_acquire(&printf_lock);

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
        print_int(va_arg(ap, int), 16, 1);
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

  spinlock_release(&printf_lock);
}