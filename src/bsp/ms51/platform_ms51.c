#include <stdbool.h>

#include "platform.h"

#include "namur_config.h"
#include "platform_ms51.h"

void ms51_gpio_set_led_mask(uint8_t mask, bool on);
bool ms51_gpio_read_dip_mask(uint8_t mask);
bool ms51_gpio_read_fault_leds_enabled(void);

static const uint8_t s_adc_map[NAMUR_NUM_CHANNELS] = {
    MS51_CH0_ADC_CHANNEL,
    MS51_CH1_ADC_CHANNEL
};

static const uint8_t s_led_mask[NAMUR_NUM_CHANNELS] = {
    MS51_CH0_LED_MASK,
    MS51_CH1_LED_MASK
};

static const uint8_t s_fault_led_mask[NAMUR_NUM_CHANNELS] = {
    MS51_CH0_FAULT_LED_MASK,
    MS51_CH1_FAULT_LED_MASK
};

static const uint8_t s_dip_mask[NAMUR_NUM_CHANNELS] = {
    MS51_CH0_DIP_MASK,
    MS51_CH1_DIP_MASK
};

void platform_init(void)
{
    ms51_clock_init();
    ms51_gpio_init();
    ms51_adc_init();
}

void platform_delay_ms(uint16_t ms)
{
    ms51_delay_ms(ms);
}

void platform_read_adc(uint8_t channel, uint16_t *adc_out)
{
    if (channel >= NAMUR_NUM_CHANNELS || adc_out == 0) {
        return;
    }
    *adc_out = ms51_adc_read_averaged(s_adc_map[channel], NAMUR_ADC_SAMPLE_COUNT);
}

bool platform_read_dip_nc(uint8_t channel)
{
    if (channel >= NAMUR_NUM_CHANNELS) {
        return false;
    }
    return ms51_gpio_read_dip_mask(s_dip_mask[channel]);
}

bool platform_read_fault_leds_enabled(void)
{
    return ms51_gpio_read_fault_leds_enabled();
}

void platform_set_channel_led(uint8_t channel, bool on)
{
    if (channel >= NAMUR_NUM_CHANNELS) {
        return;
    }
    ms51_gpio_set_led_mask(s_led_mask[channel], on);
}

void platform_set_channel_fault_led(uint8_t channel, bool on)
{
    if (channel >= NAMUR_NUM_CHANNELS) {
        return;
    }
    ms51_gpio_set_led_mask(s_fault_led_mask[channel], on);
}
