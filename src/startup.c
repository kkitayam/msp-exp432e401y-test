/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2026 Koji KITAYAMA */
#include <stddef.h>
#include <string.h>
#include <msp432e401y.h>

/* Symbols defined by the linker script */
extern const uint8_t __psp_top[];
extern const uint8_t __text_load[];
extern const uint8_t __data_load[];
extern uint8_t __text[];
extern uint8_t __etext[];
extern uint8_t __data[];
extern uint8_t __edata[];
extern uint8_t __bss[];
extern uint8_t __ebss[];

extern void SystemInit(void);
extern int main(void);

static inline void wordcpy(void *__restrict dest, const void *__restrict src, size_t n)
{
    const uint32_t *s = src;
    uint32_t *d = dest;
    n /= 4;
    while (n--) {
        *d++ = *s++;
    }
}

static inline void wordset(void *s, uint32_t v, size_t n)
{
    uint32_t *p = s;
    n /= 4;
    while (n--) {
        *p++ = v;
    }
}

void Reset_Handler(void)
{
    SystemInit();
    wordcpy(__text, __text_load, __etext - __text);
    wordcpy(__data, __data_load, __edata - __data);
    wordset(__bss, 0, __ebss - __bss);
    main();
END:
    __WFI();
    goto END;
}


void Default_Handler(void)
{
    for (;;) __WFI();
}

