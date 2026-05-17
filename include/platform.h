#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "namur_config.h"

/**
 * Hardware abstraction — MS51 implements this in platform_ms51.c.
 * Each channel uses separate ADC, status LED, fault LED, and NO/NC DIP.
 */

void platform_init(void);
void platform_delay_ms(uint16_t ms);

void platform_read_adc(uint8_t channel, uint16_t *adc_out);
bool platform_read_dip_nc(uint8_t channel);

/** true = fault LEDs may light; false = force all channel fault LEDs off (P1.2 master DIP) */
bool platform_read_fault_leds_enabled(void);

void platform_set_channel_led(uint8_t channel, bool on);
void platform_set_channel_fault_led(uint8_t channel, bool on);

#endif /* PLATFORM_H */
