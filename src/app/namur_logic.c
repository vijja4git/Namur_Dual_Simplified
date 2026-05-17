#include "namur_logic.h"

#include "namur_config.h"

uint16_t namur_adc_to_voltage_mv(uint16_t adc_avg)
{
    uint32_t mv = (uint32_t)adc_avg * (uint32_t)NAMUR_VREF_MV;
    mv /= (uint32_t)NAMUR_ADC_RESOLUTION;
    if (mv > 65535UL) {
        return 65535U;
    }
    return (uint16_t)mv;
}

uint16_t namur_adc_to_current_ua(uint16_t adc_avg)
{
    /* I(µA) = V(mV) * 1000 / R(Ω) = adc * VREF_MV * 1000 / (4096 * R) */
    uint32_t ua = (uint32_t)adc_avg * (uint32_t)NAMUR_VREF_MV * 1000UL;
    ua /= ((uint32_t)NAMUR_ADC_RESOLUTION * (uint32_t)NAMUR_SHUNT_OHM);
    if (ua > 65535UL) {
        return 65535U;
    }
    return (uint16_t)ua;
}

/* Object detected: 0.3–1 mA (voltage 0.2–0.8 V on bench; current is primary here) */
static bool namur_in_sense_band(uint16_t current_ua)
{
    return (current_ua >= NAMUR_I_SENSE_MIN_UA) &&
           (current_ua <= NAMUR_I_SENSE_MAX_UA);
}

void namur_logic_evaluate_channel(uint16_t adc_avg,
                                  bool dip_nc,
                                  bool prev_latch_on,
                                  namur_channel_state_t *state_out)
{
    uint16_t voltage_mv = namur_adc_to_voltage_mv(adc_avg);
    uint16_t current_ua = namur_adc_to_current_ua(adc_avg);
    bool latch = prev_latch_on;

    state_out->adc_avg = adc_avg;
    state_out->voltage_mv = voltage_mv;
    state_out->current_ua = current_ua;
    state_out->fault = NAMUR_FAULT_NONE;
    state_out->latch_on = latch;
    state_out->output_on = false;
    state_out->channel_led_on = false;
    state_out->fault_led_on = false;

    if ((voltage_mv < NAMUR_V_OPEN_MV) || (current_ua < NAMUR_I_OPEN_UA)) {
        state_out->fault = NAMUR_FAULT_LEAD_BREAK;
        state_out->fault_led_on = true;
        return;
    }

    if ((voltage_mv > NAMUR_V_SHORT_MV) || (current_ua > NAMUR_I_SHORT_UA)) {
        state_out->fault = NAMUR_FAULT_SHORT_CIRCUIT;
        state_out->fault_led_on = true;
        return;
    }

    if (current_ua >= NAMUR_I_IDLE_MIN_UA) {
        latch = false;
    } else if (namur_in_sense_band(current_ua)) {
        latch = true;
    }

    state_out->latch_on = latch;

    bool output = latch;
    if (dip_nc) {
        output = !latch;
    }

    state_out->output_on = output;
    state_out->channel_led_on = output;
}

void namur_logic_init(namur_system_state_t *state)
{
    uint8_t ch;

    for (ch = 0U; ch < NAMUR_NUM_CHANNELS; ch++) {
        state->channel[ch].adc_avg = 0U;
        state->channel[ch].voltage_mv = 0U;
        state->channel[ch].current_ua = 0U;
        state->channel[ch].fault = NAMUR_FAULT_NONE;
        state->channel[ch].latch_on = false;
        state->channel[ch].output_on = false;
        state->channel[ch].channel_led_on = false;
        state->channel[ch].fault_led_on = false;
    }
}
