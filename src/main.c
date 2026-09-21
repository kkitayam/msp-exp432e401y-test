/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2026 Koji KITAYAMA */
#include <msp432e401y.h>
#include "uart.h"

int main(void)
{
  uart_init();
  uart_puts("Hello, UART!\n");
  for (;;) __WFI();
}
