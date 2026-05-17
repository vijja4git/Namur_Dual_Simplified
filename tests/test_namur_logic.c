#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "namur_config.h"
#include "namur_logic.h"

static uint16_t mv_to_adc(uint16_t mv)
{
    uint32_t adc = (uint32_t)mv * (uint32_t)NAMUR_ADC_RESOLUTION;
    adc /= (uint32_t)NAMUR_VREF_MV;
    return (uint16_t)adc;
}

static uint16_t ua_to_adc(uint16_t ua)
{
    uint32_t num = (uint32_t)ua * (uint32_t)NAMUR_ADC_RESOLUTION * (uint32_t)NAMUR_SHUNT_OHM;
    uint32_t den = (uint32_t)NAMUR_VREF_MV * 1000UL;
    return (uint16_t)(num / den);
}

static void test_nominal_open(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(0U, false, false, &st);
    assert(st.fault == NAMUR_FAULT_LEAD_BREAK);
    assert(st.voltage_mv < 80U);
    assert(st.current_ua < 80U);
}

static void test_nominal_connected_idle(void)
{
    namur_channel_state_t st;
    uint16_t adc = mv_to_adc(670U);
    namur_logic_evaluate_channel(adc, false, true, &st);
    assert(st.fault == NAMUR_FAULT_NONE);
    assert(st.latch_on == false);
    assert(st.current_ua >= NAMUR_I_IDLE_MIN_UA);
}

static void test_nominal_sensing_on(void)
{
    namur_channel_state_t st;
    uint16_t adc = ua_to_adc(800U);
    namur_logic_evaluate_channel(adc, false, false, &st);
    assert(st.fault == NAMUR_FAULT_NONE);
    assert(st.latch_on == true);
    assert(st.current_ua >= NAMUR_I_SENSE_MIN_UA);
    assert(st.current_ua <= NAMUR_I_SENSE_MAX_UA);
}

static void test_nominal_short(void)
{
    namur_channel_state_t st;
    uint16_t adc = mv_to_adc(1550U);
    namur_logic_evaluate_channel(adc, false, false, &st);
    assert(st.fault == NAMUR_FAULT_SHORT_CIRCUIT);
    assert(st.voltage_mv > NAMUR_V_SHORT_MV);
}

static void test_hysteresis_hold_mid_current(void)
{
    namur_channel_state_t st;
    uint16_t adc = ua_to_adc(2000U);
    namur_logic_evaluate_channel(adc, false, true, &st);
    assert(st.latch_on == true);
    namur_logic_evaluate_channel(adc, false, false, &st);
    assert(st.latch_on == false);
}

static void test_dip_no_vs_nc(void)
{
    namur_channel_state_t st;
    uint16_t adc = ua_to_adc(800U);
    namur_logic_evaluate_channel(adc, false, false, &st);
    assert(st.output_on == true);
    namur_logic_evaluate_channel(adc, true, false, &st);
    assert(st.output_on == false);
}

static void test_channel_independence(void)
{
    namur_channel_state_t st0;
    namur_channel_state_t st1;
    namur_logic_evaluate_channel(0U, false, false, &st0);
    namur_logic_evaluate_channel(ua_to_adc(800U), false, false, &st1);
    assert(st0.fault == NAMUR_FAULT_LEAD_BREAK);
    assert(st1.latch_on == true);
}

static void test_sense_at_1ma(void)
{
    namur_channel_state_t st;
    uint16_t adc = ua_to_adc(1000U);
    namur_logic_evaluate_channel(adc, false, false, &st);
    assert(st.fault == NAMUR_FAULT_NONE);
    assert(st.latch_on == true);
}

int main(void)
{
    test_nominal_open();
    test_nominal_connected_idle();
    test_nominal_sensing_on();
    test_nominal_short();
    test_hysteresis_hold_mid_current();
    test_dip_no_vs_nc();
    test_channel_independence();
    test_sense_at_1ma();
    printf("All namur_logic tests passed.\n");
    return 0;
}
