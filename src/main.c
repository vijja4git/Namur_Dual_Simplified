#include <stdbool.h>

#include "namur_config.h"
#include "namur_logic.h"
#include "platform.h"

static namur_system_state_t g_state;

int main(void)
{
    platform_init();
    namur_logic_init(&g_state);

    for (;;) {
        uint8_t ch;

        for (ch = 0U; ch < NAMUR_NUM_CHANNELS; ch++) {
            uint16_t adc_avg;
            uint16_t current_ua;
            bool dip_nc;
            namur_channel_state_t *ch_state = &g_state.channel[ch];
            bool prev_latch = ch_state->latch_on;

            platform_read_adc(ch, &adc_avg);
            current_ua = namur_adc_to_current_ua(adc_avg);
            dip_nc = platform_read_dip_nc(ch);

            namur_logic_evaluate_channel(current_ua, dip_nc, prev_latch, ch_state);

            if (ch_state->fault != NAMUR_FAULT_NONE) {
                ch_state->channel_led_on = false;
                platform_set_channel_led(ch, false);
            } else {
                platform_set_channel_led(ch, ch_state->channel_led_on);
            }
        }

        g_state.global_fault_led_on = namur_logic_any_fault(&g_state);
        platform_set_fault_led(g_state.global_fault_led_on);

        platform_delay_ms(NAMUR_LOOP_PERIOD_MS);
    }
}
