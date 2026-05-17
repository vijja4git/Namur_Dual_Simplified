#include <stdbool.h>

#include "platform_ms51.h"

#include "ms51_registers.h"

static uint8_t s_p0_out;

void ms51_gpio_init(void)
{
    /* Status + fault LEDs on P0.0–P0.3: push-pull outputs */
    P0M1 &= (uint8_t)~MS51_P0_LED_OUTPUT_MASK;
    P0M2 |= (uint8_t)MS51_P0_LED_OUTPUT_MASK;

    /* Per-channel NO/NC DIPs on P1.0, P1.1; master fault-enable on P1.2 */
    P1M1 |= (uint8_t)(MS51_CH0_DIP_MASK | MS51_CH1_DIP_MASK | MS51_FAULT_LED_ENABLE_MASK);
    P1M2 &= (uint8_t)~(MS51_CH0_DIP_MASK | MS51_CH1_DIP_MASK | MS51_FAULT_LED_ENABLE_MASK);

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
    return (P1 & mask) != 0U;
}

bool ms51_gpio_read_fault_leds_enabled(void)
{
    /* DIP open (HIGH) = allow fault LEDs; DIP closed to GND (LOW) = disable all fault LEDs */
    return (P1 & MS51_FAULT_LED_ENABLE_MASK) != 0U;
}
