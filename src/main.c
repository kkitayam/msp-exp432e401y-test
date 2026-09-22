/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2026 Koji KITAYAMA */
#include <msp432e401y.h>
#include "uart.h"

extern int test_code_flash(void);
extern int test_data_flash(void);

static void uart_put_uint(const char* s, uint32_t value, bool negative)
{
    char buf[32]; /* int32_t max is 11 digits + sign + null terminator */
    char *p = buf + sizeof(buf) - 1;
    *p = '\0';

    do {
        *--p = '0' + (value % 10);
        value /= 10;
    } while (value != 0);

    if (negative) {
        *--p = '-';
    }
    uart_puts(s);
    uart_puts(p);
    uart_putc('\n');
}

void uart_put_digit(const char* s, uint32_t value)
{
    uart_put_uint(s, value, false);
}

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t DWT_GetCycles(void)
{
    return DWT->CYCCNT;
}

int main(void)
{
  DWT_Init();
  uart_init();
  uart_puts("Hello, UART!\n");
  uint32_t beg, end;
  int err;
  beg = DWT_GetCycles();
  err = test_code_flash();
  end = DWT_GetCycles();
  if (err < 0)
    uart_puts("Code flash test failed!\n");
  else
    uart_puts("Code flash test passed!\n");
  uart_put_digit("Code flash test cycles: ", end - beg);
  
  beg = DWT_GetCycles();
  err = test_data_flash();
  end = DWT_GetCycles();
  if (err < 0)
    uart_puts("Data flash test failed!\n");
  else
    uart_puts("Data flash test passed!\n");
  uart_put_digit("Data flash test cycles: ", end - beg);

  for (;;) __WFI();
}
