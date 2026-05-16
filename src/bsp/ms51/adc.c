#include "platform_ms51.h"

#include "namur_config.h"
#include "ms51_registers.h"

void ms51_adc_init(void)
{
    ADCCON1 = ADCEN;
    ADCCON0 = 0x00U;
    ADCCON2 = 0x00U;
    ADCDLY = 0x00U;
}

static uint16_t ms51_adc_read_once(uint8_t adc_channel)
{
    uint8_t ch = (uint8_t)(adc_channel & ADCHS_MASK);

    ADCCON0 = (uint8_t)((ADCCON0 & (uint8_t)~ADCHS_MASK) | ch);
    ADCCON0 |= ADCS;

    while ((ADCCON0 & ADCF) == 0U) {
        /* poll */
    }

    return (uint16_t)(((uint16_t)ADCRH << 4) | ((uint16_t)ADCRL & 0x0FU));
}

uint16_t ms51_adc_read_averaged(uint8_t adc_channel, uint8_t sample_count)
{
    uint32_t sum = 0U;
    uint8_t i;

    if (sample_count == 0U) {
        sample_count = 1U;
    }

    for (i = 0U; i < sample_count; i++) {
        sum += ms51_adc_read_once(adc_channel);
    }

    return (uint16_t)(sum / (uint32_t)sample_count);
}
