#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "namur_config.h"

/**
 * Platform abstraction — implemented in src/bsp/ms51 for firmware,
 * tests/mocks/mock_platform.c for host tests.
 */

void platform_init(void);
void platform_delay_ms(uint16_t ms);

/** Read averaged 12-bit ADC counts per channel (already averaged in BSP). */
void platform_read_adc(uint8_t channel, uint16_t *adc_out);

/** DIP switch: true = NC (invert in normal state), false = NO (follow). */
bool platform_read_dip_nc(uint8_t channel);

void platform_set_channel_led(uint8_t channel, bool on);
void platform_set_fault_led(bool on);

#endif /* PLATFORM_H */
