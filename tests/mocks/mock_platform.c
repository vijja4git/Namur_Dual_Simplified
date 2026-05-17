#include "platform.h"

#include <string.h>

static uint16_t s_adc[NAMUR_NUM_CHANNELS];
static bool s_dip_nc[NAMUR_NUM_CHANNELS];
static bool s_ch_led[NAMUR_NUM_CHANNELS];
static bool s_ch_fault_led[NAMUR_NUM_CHANNELS];
static bool s_fault_leds_enabled = true;

void mock_platform_reset(void)
{
    memset(s_adc, 0, sizeof(s_adc));
    memset(s_dip_nc, 0, sizeof(s_dip_nc));
    memset(s_ch_led, 0, sizeof(s_ch_led));
    memset(s_ch_fault_led, 0, sizeof(s_ch_fault_led));
    s_fault_leds_enabled = true;
}

void mock_platform_set_adc(uint8_t channel, uint16_t adc_avg)
{
    if (channel < NAMUR_NUM_CHANNELS) {
        s_adc[channel] = adc_avg;
    }
}

void mock_platform_set_dip_nc(uint8_t channel, bool nc)
{
    if (channel < NAMUR_NUM_CHANNELS) {
        s_dip_nc[channel] = nc;
    }
}

void mock_platform_set_fault_leds_enabled(bool enabled)
{
    s_fault_leds_enabled = enabled;
}

bool mock_platform_get_channel_led(uint8_t channel)
{
    if (channel >= NAMUR_NUM_CHANNELS) {
        return false;
    }
    return s_ch_led[channel];
}

bool mock_platform_get_channel_fault_led(uint8_t channel)
{
    if (channel >= NAMUR_NUM_CHANNELS) {
        return false;
    }
    return s_ch_fault_led[channel];
}

void platform_init(void) {}
void platform_delay_ms(uint16_t ms) { (void)ms; }

void platform_read_adc(uint8_t channel, uint16_t *adc_out)
{
    if (channel < NAMUR_NUM_CHANNELS && adc_out != 0) {
        *adc_out = s_adc[channel];
    }
}

bool platform_read_dip_nc(uint8_t channel)
{
    if (channel < NAMUR_NUM_CHANNELS) {
        return s_dip_nc[channel];
    }
    return false;
}

bool platform_read_fault_leds_enabled(void)
{
    return s_fault_leds_enabled;
}

void platform_set_channel_led(uint8_t channel, bool on)
{
    if (channel < NAMUR_NUM_CHANNELS) {
        s_ch_led[channel] = on;
    }
}

void platform_set_channel_fault_led(uint8_t channel, bool on)
{
    if (channel < NAMUR_NUM_CHANNELS) {
        s_ch_fault_led[channel] = on;
    }
}
