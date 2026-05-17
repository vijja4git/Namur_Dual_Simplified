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
        bool fault_leds_enabled = platform_read_fault_leds_enabled(); /* P1.2 master DIP */

        for (ch = 0U; ch < NAMUR_NUM_CHANNELS; ch++) {
            uint16_t adc_avg;
            uint16_t current_ua;
            bool dip_nc;
            namur_channel_state_t *ch_state = &g_state.channel[ch];
            bool prev_latch = ch_state->latch_on;
            bool drive_fault_led;

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

            /* This channel's fault LED only — never coupled to the other channel */
            drive_fault_led = ch_state->fault_led_on && fault_leds_enabled;
            platform_set_channel_fault_led(ch, drive_fault_led);
        }

        platform_delay_ms(NAMUR_LOOP_PERIOD_MS);
    }
}
