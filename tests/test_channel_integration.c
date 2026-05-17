#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "namur_app.h"
#include "namur_config.h"
#include "namur_logic.h"
#include "mock_platform.h"

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

static void set_both_adc_raw(uint16_t adc0, uint16_t adc1)
{
    mock_platform_set_adc(0U, adc0);
    mock_platform_set_adc(1U, adc1);
}

static void set_both_mv(uint16_t mv_ch0, uint16_t mv_ch1)
{
    set_both_adc_raw(mv_to_adc(mv_ch0), mv_to_adc(mv_ch1));
}

static void test_ch0_open_ch1_sensing(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_fault_leds_enabled(true);
    mock_platform_set_dip_nc(0U, false);
    mock_platform_set_dip_nc(1U, false);
    namur_logic_init(&state);

    set_both_adc_raw(0U, ua_to_adc(800U));
    namur_app_tick(&state);

    assert(state.channel[0].fault == NAMUR_FAULT_LEAD_BREAK);
    assert(mock_platform_get_channel_led(0U) == false);
    assert(mock_platform_get_channel_fault_led(0U) == true);

    assert(state.channel[1].fault == NAMUR_FAULT_NONE);
    assert(state.channel[1].latch_on == true);
    assert(mock_platform_get_channel_led(1U) == true);
    assert(mock_platform_get_channel_fault_led(1U) == false);
}

static void test_varying_sense_and_idle(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_dip_nc(0U, false);
    mock_platform_set_adc(1U, mv_to_adc(670U));
    namur_logic_init(&state);

    mock_platform_set_adc(0U, mv_to_adc(670U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == false);
    assert(mock_platform_get_channel_led(0U) == false);

    mock_platform_set_adc(0U, ua_to_adc(800U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == true);
    assert(mock_platform_get_channel_led(0U) == true);

    mock_platform_set_adc(0U, ua_to_adc(2000U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == true);

    mock_platform_set_adc(0U, mv_to_adc(670U));
    namur_app_tick(&state);
    assert(state.channel[0].latch_on == false);
}

static void test_no_vs_nc_sensing(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_adc(0U, ua_to_adc(800U));
    namur_logic_init(&state);

    mock_platform_set_dip_nc(0U, false);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_led(0U) == true);

    mock_platform_set_dip_nc(0U, true);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_led(0U) == false);
}

static void test_short_only_ch1(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    mock_platform_set_fault_leds_enabled(true);
    set_both_mv(670U, 1550U);
    namur_logic_init(&state);

    namur_app_tick(&state);

    assert(state.channel[0].fault == NAMUR_FAULT_NONE);
    assert(mock_platform_get_channel_fault_led(0U) == false);
    assert(state.channel[1].fault == NAMUR_FAULT_SHORT_CIRCUIT);
    assert(mock_platform_get_channel_fault_led(1U) == true);
}

static void test_master_dip_disables_fault_leds(void)
{
    namur_system_state_t state;

    mock_platform_reset();
    set_both_adc_raw(0U, mv_to_adc(1550U));
    namur_logic_init(&state);

    mock_platform_set_fault_leds_enabled(true);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_fault_led(0U) == true);
    assert(mock_platform_get_channel_fault_led(1U) == true);

    mock_platform_set_fault_leds_enabled(false);
    namur_app_tick(&state);
    assert(mock_platform_get_channel_fault_led(0U) == false);
    assert(mock_platform_get_channel_fault_led(1U) == false);
}

int main(void)
{
    test_ch0_open_ch1_sensing();
    test_varying_sense_and_idle();
    test_no_vs_nc_sensing();
    test_short_only_ch1();
    test_master_dip_disables_fault_leds();
    printf("All channel integration tests passed.\n");
    return 0;
}
