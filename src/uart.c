/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2026 Koji KITAYAMA */
#include <stddef.h>
#include <msp432e401y.h>

void uart_init(void)
{
  /* UART PA0,1 */
  const unsigned bits = 1u << 0;
  SYSCTL->RCGCGPIO |= bits;
  while (bits != (SYSCTL->RCGCGPIO & bits)) ;
  GPIOA->AFSEL = 3u;
  GPIOA->PCTL  = 0x11u;
  GPIOA->DEN   = 3u;

  SYSCTL->RCGCUART |= 1u << 0;
  while (!(SYSCTL->PRUART & (1u << 0))) ;
  UART0->CTL  = 0;
  UART0->IBRD = 8;  /* 8.68056 = 16MHz / (16 * 115200) */
  UART0->FBRD = 44; /* 0.6875 = 44/64 -> 115108bps (0.08%) */
  UART0->LCRH = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
  UART0->CC   = UART_CC_CS_PIOSC; /* Set the baud clock to PIOSC */
  UART0->CTL  = UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;
}

void uart_putc(char c)
{
  while (UART0->FR & UART_FR_TXFF) ;
  UART0->DR = c;
}

void uart_puts(const char* str)
{
  if (str == NULL) return;
  char c;
  while ((c = *str)) {
    if ('\n' == c)
      uart_putc('\r'); /* CR+LF */
    uart_putc(c);
    str++;
  }
  /* wait for completion */
  while (UART0->FR & UART_FR_BUSY) ;
}
