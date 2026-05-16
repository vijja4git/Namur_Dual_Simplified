#include "namur_logic.h"

#include "namur_config.h"

uint16_t namur_adc_to_current_ua(uint16_t adc_avg)
{
    /*
     * I(µA) = (adc / 4096) * Vref(V) * 1e6 / R(Ω)
     *       = adc * VREF_MV * 1000 / (4096 * SHUNT_mΩ) * 1000
     * With VREF_MV=5000, SHUNT=500Ω => adc * 10000 / 4096
     */
    uint32_t ua = (uint32_t)adc_avg * 10000UL;
    ua /= (uint32_t)NAMUR_ADC_RESOLUTION;
    if (ua > 65535UL) {
        return 65535U;
    }
    return (uint16_t)ua;
}

void namur_logic_evaluate_channel(uint16_t current_ua,
                                  bool dip_nc,
                                  bool prev_latch_on,
                                  namur_channel_state_t *state_out)
{
    bool latch = prev_latch_on;

    state_out->current_ua = current_ua;
    state_out->fault = NAMUR_FAULT_NONE;
    state_out->latch_on = latch;
    state_out->output_on = false;
    state_out->channel_led_on = false;

    if (current_ua < NAMUR_THRESH_FAULT_OPEN_UA) {
        state_out->fault = NAMUR_FAULT_LEAD_BREAK;
        state_out->latch_on = latch;
        return;
    }

    if (current_ua > NAMUR_THRESH_FAULT_SHORT_UA) {
        state_out->fault = NAMUR_FAULT_SHORT_CIRCUIT;
        state_out->latch_on = latch;
        return;
    }

    if (current_ua > NAMUR_THRESH_ON_UA) {
        latch = true;
    } else if (current_ua < NAMUR_THRESH_OFF_UA) {
        latch = false;
    }

    state_out->latch_on = latch;

    bool output = latch;
    if (dip_nc) {
        output = !latch;
    }

    state_out->output_on = output;
    state_out->channel_led_on = output;
}

bool namur_logic_any_fault(const namur_system_state_t *state)
{
    uint8_t ch;

    for (ch = 0U; ch < NAMUR_NUM_CHANNELS; ch++) {
        if (state->channel[ch].fault != NAMUR_FAULT_NONE) {
            return true;
        }
    }
    return false;
}

void namur_logic_init(namur_system_state_t *state)
{
    uint8_t ch;

    for (ch = 0U; ch < NAMUR_NUM_CHANNELS; ch++) {
        state->channel[ch].current_ua = 0U;
        state->channel[ch].fault = NAMUR_FAULT_NONE;
        state->channel[ch].latch_on = false;
        state->channel[ch].output_on = false;
        state->channel[ch].channel_led_on = false;
    }
    state->global_fault_led_on = false;
}

