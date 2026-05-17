#ifndef MOCK_PLATFORM_H
#define MOCK_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

void mock_platform_reset(void);
void mock_platform_set_adc(uint8_t channel, uint16_t adc_avg);
void mock_platform_set_dip_nc(uint8_t channel, bool nc);
void mock_platform_set_fault_leds_enabled(bool enabled);

bool mock_platform_get_channel_led(uint8_t channel);
bool mock_platform_get_channel_fault_led(uint8_t channel);

#endif /* MOCK_PLATFORM_H */
