#include "platform_ms51.h"

#include "ms51_registers.h"

void ms51_clock_init(void)
{
    /*
     * Default: internal 24 MHz HIRC after reset (see TRM clock chapter).
     * Extended SFRs (CLKDIV/CKCON) require Nuvoton paging helpers; not
     * needed for bring-up — delay.c is calibrated for this default.
     */
}
