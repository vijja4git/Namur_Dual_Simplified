#include "platform_ms51.h"  /* ms51_adc_init, read declarations */

#include "namur_config.h"   /* NAMUR_ADC_SAMPLE_COUNT if needed by callers */
#include "ms51_registers.h" /* ADCCON0, ADCRH, etc. */

/* Turn on the MS51 12-bit ADC peripheral once at boot */
void ms51_adc_init(void)
{
    ADCCON1 = ADCEN;   /* ADCEN bit: enable ADC block */
    ADCCON0 = 0x00U;   /* Clear control: no conversion in progress */
    ADCCON2 = 0x00U;   /* Extra ADC options default off */
    ADCDLY = 0x00U;    /* No trigger delay */
}

/* One conversion on adc_channel (0 = CH0/P1.7, 1 = CH1/P3.0) */
static uint16_t ms51_adc_read_once(uint8_t adc_channel)
{
    uint8_t ch = (uint8_t)(adc_channel & ADCHS_MASK); /* Mask to 4-bit channel select */

    ADCCON0 = (uint8_t)((ADCCON0 & (uint8_t)~ADCHS_MASK) | ch); /* Select analog input mux */
    ADCCON0 |= ADCS;              /* ADCS=1: start conversion */

    while ((ADCCON0 & ADCF) == 0U) { /* ADCF=1 when conversion complete */
        /* Busy-wait (short); loop is 20 ms so this is acceptable */
    }

    /* 12-bit result: high nibble in ADCRH, low nibble in ADCRL */
    return (uint16_t)(((uint16_t)ADCRH << 4) | ((uint16_t)ADCRL & 0x0FU));
}

/* Average sample_count readings to reduce noise on the shunt voltage */
uint16_t ms51_adc_read_averaged(uint8_t adc_channel, uint8_t sample_count)
{
    uint32_t sum = 0U;            /* Use 32-bit to avoid overflow over 8 samples */
    uint8_t i;

    if (sample_count == 0U) {     /* Guard against divide-by-zero */
        sample_count = 1U;
    }

    for (i = 0U; i < sample_count; i++) {
        sum += ms51_adc_read_once(adc_channel); /* Add one reading */
    }

    return (uint16_t)(sum / (uint32_t)sample_count); /* Integer average → app converts to µA */
}
