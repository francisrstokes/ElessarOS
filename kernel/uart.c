#include "inc/uart.h"
#include "inc/proc.h"
#include "inc/spinlock.h"

static spinlock_t uart_tx_lock;

char uart_tx_buf[UART_TX_BUF_SIZE];

void uart_init(void) {
  // disable interrupts.
  UART_REG(IER) = 0x00;

  // special mode to set baud rate.
  UART_REG(LCR) = LCR_BAUD_LATCH;

  // baud rate of 38.4K.
  UART_REG(LCR_BAUD_LSB) = 0x03;
  UART_REG(LCR_BAUD_MSB) = 0x00;

  // leave set-baud mode,
  // and set word length to 8 bits, no parity.
  UART_REG(LCR) = LCR_EIGHT_BITS;

  // reset and enable FIFOs.
  UART_REG(FCR) = FCR_FIFO_ENABLE | FCR_FIFO_CLEAR;

  // TODO: enable transmit and receive interrupts.
  // UART_REG(IER) = IER_TX_ENABLE | IER_RX_ENABLE;

  spinlock_init(&uart_tx_lock, "uart_tx");
}

void uart_write(i32 ch) {
  spinlock_acquire(&uart_tx_lock);

  while((UART_REG(LSR) & LSR_TX_IDLE) == 0) {
    // Wait until ready to transmit
  }
  UART_REG(THR) = ch;

  spinlock_release(&uart_tx_lock);
}

// Non-interrupt based, blocking version of uart_write that can be used by the kernel
void uart_kwrite(i32 ch) {
  acquire_push_interrupt_state();

  while((UART_REG(LSR) & LSR_TX_IDLE) == 0) {
    // Wait until ready to transmit
  }

  UART_REG(THR) = ch;

  release_pop_interrupt_state();
}
