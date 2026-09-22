#include "msp432e401y_flash.h"
#include <string.h>
#include <stdint.h>

/* Last 16 KiB sector of upper bank (logical bank 1).
 * Caller must ensure this test runs from RAM or from the opposite bank. */
#define TEST_ADDR   0x000FC000U
#define TEST_SIZE   0x100U

static uint8_t pattern[TEST_SIZE];

int test_code_flash(void)
{
    size_t i;

    for (i = 0; i < TEST_SIZE; i++)
        pattern[i] = (uint8_t)(0x5A ^ (i & 0xFF));

    msp432_flash_init();

    if (msp432_flash_erase(TEST_ADDR, 0x4000U) != 0)
        return -1;
    if (!msp432_flash_is_blank(TEST_ADDR, TEST_SIZE))
        return -2;

    if (msp432_flash_write(TEST_ADDR, pattern, TEST_SIZE) != 0)
        return -3;
    if (memcmp((const void *)TEST_ADDR, pattern, TEST_SIZE) != 0)
        return -4;
    if (msp432_flash_is_blank(TEST_ADDR, TEST_SIZE))
        return -5;

    return 0;
}
