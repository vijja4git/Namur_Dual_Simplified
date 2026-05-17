#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "namur_app.h"
#include "namur_config.h"
#include "namur_logic.h"
#include "mock_platform.h"

static uint16_t ua_to_adc(uint16_t ua)
{
    uint32_t adc = (uint32_t)ua * (uint32_t)NAMUR_ADC_RESOLUTION;
    return (uint16_t)(adc / 10000UL);
}

static void set_both_adc(uint16_t ua_ch0, uint16_t ua_ch1)
{
    mock_platform_set_adc(0U, ua_to_adc(ua_ch0));
    mock_platform_set_adc(1U, ua_to_adc(ua_ch1));
}

static void test_ch0_fault_does_not_affect_ch1_leds(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_fault_leds_enabled(true);
    mock_platform_set_dip_nc(0U, false);
    mock_platform_set_dip_nc(1U, false);
    namur_logic_init(&state);

    set_both_adc(100U, 2500U);
    namur_app_tick(&state);

    assert(state.channel[0].fault == NAMUR_FAULT_LEAD_BREAK);
    assert(mock_platform_get_channel_led(0U) == false);
    assert(mock_platform_get_channel_fault_led(0U) == true);

    assert(state.channel[1].fault == NAMUR_FAULT_NONE);
    assert(state.channel[1].latch_on == true);
    assert(mock_platform_get_channel_led(1U) == true);
    assert(mock_platform_get_channel_fault_led(1U) == false);
}

static void test_varying_current_hysteresis_ch0(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_dip_nc(0U, false);
    mock_platform_set_adc(1U, ua_to_adc(800U));
    namur_logic_init(&state);

    mock_platform_set_adc(0U, ua_to_adc(1100U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == false);
    assert(mock_platform_get_channel_led(0U) == false);

    mock_platform_set_adc(0U, ua_to_adc(2200U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == true);
    assert(mock_platform_get_channel_led(0U) == true);

    mock_platform_set_adc(0U, ua_to_adc(1500U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == true);
    assert(mock_platform_get_channel_led(0U) == true);

    mock_platform_set_adc(0U, ua_to_adc(1100U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == false);
    assert(mock_platform_get_channel_led(0U) == false);
}

static void test_no_vs_nc_same_current(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_adc(0U, ua_to_adc(2200U));
    namur_logic_init(&state);

    mock_platform_set_dip_nc(0U, false);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_led(0U) == true);

    mock_platform_set_dip_nc(0U, true);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_led(0U) == false);

    mock_platform_set_dip_nc(0U, false);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_led(0U) == true);
}

static void test_nc_does_not_apply_during_fault(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_dip_nc(0U, true);
    mock_platform_set_adc(0U, ua_to_adc(100U));
    namur_logic_init(&state);

    namur_app_tick(&state);
    assert(state.channel[0].fault == NAMUR_FAULT_LEAD_BREAK);
    assert(mock_platform_get_channel_led(0U) == false);
}

static void test_short_circuit_only_ch1_fault_led(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_fault_leds_enabled(true);
    set_both_adc(2500U, 7000U);
    namur_logic_init(&state);

    namur_app_tick(&state);

    assert(state.channel[0].fault == NAMUR_FAULT_NONE);
    assert(mock_platform_get_channel_fault_led(0U) == false);
    assert(state.channel[1].fault == NAMUR_FAULT_SHORT_CIRCUIT);
    assert(mock_platform_get_channel_led(1U) == false);
    assert(mock_platform_get_channel_fault_led(1U) == true);
}

static void test_master_dip_disables_fault_leds_only(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    set_both_adc(100U, 7000U);
    namur_logic_init(&state);

    mock_platform_set_fault_leds_enabled(true);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_fault_led(0U) == true);
    assert(mock_platform_get_channel_fault_led(1U) == true);

    mock_platform_set_fault_leds_enabled(false);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_fault_led(0U) == false);
    assert(mock_platform_get_channel_fault_led(1U) == false);
    assert(state.channel[0].fault == NAMUR_FAULT_LEAD_BREAK);
    assert(state.channel[1].fault == NAMUR_FAULT_SHORT_CIRCUIT);
}

static void test_dual_channel_mixed_states(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_fault_leds_enabled(true);
    mock_platform_set_dip_nc(0U, false);
    mock_platform_set_dip_nc(1U, true);
    set_both_adc(2200U, 1100U);
    namur_logic_init(&state);

    namur_app_tick(&state);

    assert(mock_platform_get_channel_led(0U) == true);
    assert(mock_platform_get_channel_fault_led(0U) == false);
    assert(state.channel[1].latch_on == false);
    assert(mock_platform_get_channel_led(1U) == true);
    assert(mock_platform_get_channel_fault_led(1U) == false);
}

int main(void)
{
    test_ch0_fault_does_not_affect_ch1_leds();
    test_varying_current_hysteresis_ch0();
    test_no_vs_nc_same_current();
    test_nc_does_not_apply_during_fault();
    test_short_circuit_only_ch1_fault_led();
    test_master_dip_disables_fault_leds_only();
    test_dual_channel_mixed_states();
    printf("All channel integration tests passed.\n");
    return 0;
}
