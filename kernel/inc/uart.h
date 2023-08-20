#ifndef INC_UART_H
#define INC_UART_H

#include "types.h"
#include "memory-map.h"

#define UART_REG(reg) (*(volatile u8*)(UART_BASE + reg))

#define UART_TX_BUF_SIZE  (32)

// Mostly taken directly from xv6
#define RHR 0                 // receive holding register (for input bytes)
#define THR 0                 // transmit holding register (for output bytes)
#define LCR_BAUD_LSB  0       // low byte baudrate register in baud latch mode
#define LCR_BAUD_MSB  1       // high byte baudrate register in baud latch mode
#define IER 1                 // interrupt enable register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR 2                 // FIFO control register
#define FCR_FIFO_ENABLE (1<<0)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define ISR 2                 // interrupt status register
#define LCR 3                 // line control register
#define LCR_EIGHT_BITS (3<<0)
#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LSR 5                 // line status register
#define LSR_RX_READY (1<<0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1<<5)    // THR can accept another character to send

void uart_init(void);
void uart_kwrite(i32 ch);
void uart_write(i32 ch);

#endif // INC_UART_H
