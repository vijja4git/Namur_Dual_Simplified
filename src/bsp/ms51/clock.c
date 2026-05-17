#include "platform_ms51.h"

void ms51_clock_init(void)
{
    /*
     * No extended clock registers programmed here (SDCC uses reset defaults).
     * After reset the MS51 typically runs internal HIRC (~24 MHz).
     * delay.c assumes that speed; change MS51_DELAY_CYCLES_PER_MS if you change clocks.
     */
}
