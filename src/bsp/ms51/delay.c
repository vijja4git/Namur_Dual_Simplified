#include "platform_ms51.h"

/*
 * Busy-wait delay calibrated for ~24 MHz HIRC, CLKDIV=1.
 * Adjust MS51_DELAY_CYCLES_PER_MS if clock source changes.
 */
#define MS51_DELAY_CYCLES_PER_MS  2400U

void ms51_delay_ms(uint16_t ms)
{
    uint16_t m;
    for (m = 0U; m < ms; m++) {
        uint16_t c = MS51_DELAY_CYCLES_PER_MS;
        while (c != 0U) {
            c--;
        }
    }
}
