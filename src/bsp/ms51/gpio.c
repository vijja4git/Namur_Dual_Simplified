#include <stdbool.h>

#include "platform_ms51.h"

#include "ms51_registers.h"

static uint8_t s_p0_out;

void ms51_gpio_init(void)
{
    /* LEDs: push-pull outputs on P0.0, P0.1, P0.2 (active high) */
    P0M1 &= (uint8_t)~(MS51_CH0_LED_MASK | MS51_CH1_LED_MASK | MS51_FAULT_LED_MASK);
    P0M2 |= (uint8_t)(MS51_CH0_LED_MASK | MS51_CH1_LED_MASK | MS51_FAULT_LED_MASK);

    /* DIP inputs with pull-up: P1.0, P1.1 */
    P1M1 |= (uint8_t)(MS51_CH0_DIP_MASK | MS51_CH1_DIP_MASK);
    P1M2 &= (uint8_t)~(MS51_CH0_DIP_MASK | MS51_CH1_DIP_MASK);

    /* ADC inputs: P1.7 (CH0), P3.0 (CH1) — high-impedance input */
    P1M1 |= 0x80U;
    P1M2 &= (uint8_t)~0x80U;
    P3M1 |= 0x01U;
    P3M2 &= (uint8_t)~0x01U;

    s_p0_out = 0x00U;
    P0 = s_p0_out;
}

void ms51_gpio_set_led_mask(uint8_t mask, bool on)
{
    if (on) {
        s_p0_out |= mask;
    } else {
        s_p0_out &= (uint8_t)~mask;
    }
    P0 = s_p0_out;
}

bool ms51_gpio_read_dip_mask(uint8_t mask)
{
    /* DIP closed to GND reads 0; open (NC label) reads 1 with pull-up */
    return (P1 & mask) != 0U;
}
