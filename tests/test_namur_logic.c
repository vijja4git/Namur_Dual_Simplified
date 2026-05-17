#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "namur_config.h"
#include "namur_logic.h"

static uint16_t ua_to_adc(uint16_t ua)
{
    uint32_t adc = (uint32_t)ua * (uint32_t)NAMUR_ADC_RESOLUTION;
    return (uint16_t)(adc / 10000UL);
}

static void test_adc_conversion(void)
{
    uint16_t ua = namur_adc_to_current_ua(2048U);
    assert(ua >= 4990U && ua <= 5010U);
}

static void test_lead_break(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(100U, false, true, &st);
    assert(st.fault == NAMUR_FAULT_LEAD_BREAK);
    assert(st.channel_led_on == false);
    assert(st.fault_led_on == true);
}

static void test_short_circuit(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(7000U, false, false, &st);
    assert(st.fault == NAMUR_FAULT_SHORT_CIRCUIT);
    assert(st.fault_led_on == true);
}

static void test_hysteresis_latch_on(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(2200U, false, false, &st);
    assert(st.fault == NAMUR_FAULT_NONE);
    assert(st.latch_on == true);
    assert(st.fault_led_on == false);
}

static void test_hysteresis_latch_off(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(1100U, false, true, &st);
    assert(st.fault == NAMUR_FAULT_NONE);
    assert(st.latch_on == false);
}

static void test_hysteresis_hold(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(1500U, false, true, &st);
    assert(st.latch_on == true);
    namur_logic_evaluate_channel(1500U, false, false, &st);
    assert(st.latch_on == false);
}

static void test_dip_no_vs_nc(void)
{
    namur_channel_state_t st;
    namur_logic_evaluate_channel(2200U, false, false, &st);
    assert(st.output_on == true);
    namur_logic_evaluate_channel(2200U, true, false, &st);
    assert(st.output_on == false);
}

static void test_channel_independence(void)
{
    namur_system_state_t sys;
    namur_channel_state_t st0;
    namur_channel_state_t st1;

    namur_logic_init(&sys);
    namur_logic_evaluate_channel(100U, false, false, &st0);
    namur_logic_evaluate_channel(2200U, false, false, &st1);

    assert(st0.fault == NAMUR_FAULT_LEAD_BREAK);
    assert(st0.fault_led_on == true);
    assert(st1.fault == NAMUR_FAULT_NONE);
    assert(st1.latch_on == true);
    assert(st1.fault_led_on == false);
}

static void test_adc_threshold_mapping(void)
{
    namur_channel_state_t st;
    uint16_t adc_mid = ua_to_adc(2500U);
    uint16_t ua_mid = namur_adc_to_current_ua(adc_mid);

    assert(ua_mid >= 2400U && ua_mid <= 2600U);

    namur_logic_evaluate_channel(149U, false, false, &st);
    assert(st.fault == NAMUR_FAULT_LEAD_BREAK);

    namur_logic_evaluate_channel(2101U, false, false, &st);
    assert(st.latch_on == true);
}

int main(void)
{
    test_adc_conversion();
    test_lead_break();
    test_short_circuit();
    test_hysteresis_latch_on();
    test_hysteresis_latch_off();
    test_hysteresis_hold();
    test_dip_no_vs_nc();
    test_channel_independence();
    test_adc_threshold_mapping();
    printf("All namur_logic tests passed.\n");
    return 0;
}
