#include "platform_ms51.h"

/*
 * Busy-wait milliseconds — not exact, but stable enough for 20 ms loop pacing.
 * Tuned for ~24 MHz internal clock after reset (see clock.c).
 */
#define MS51_DELAY_CYCLES_PER_MS  2400U /* Inner loop iterations ≈ 1 ms at 24 MHz */

void ms51_delay_ms(uint16_t ms)
{
    uint16_t m;
    for (m = 0U; m < ms; m++) {           /* Outer: count milliseconds */
        uint16_t c = MS51_DELAY_CYCLES_PER_MS;
        while (c != 0U) {                 /* Inner: burn CPU cycles */
            c--;
        }
    }
}
