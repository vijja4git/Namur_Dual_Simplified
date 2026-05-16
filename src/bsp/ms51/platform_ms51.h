#ifndef PLATFORM_MS51_H
#define PLATFORM_MS51_H

#include <stdint.h>

#include "ms51_registers.h"

/* Pin map for documented reference hardware (see docs/HARDWARE_CONNECTIONS.md) */

/* Channel 1: ADC_CH0 @ P1.7, LED P0.0, DIP NC @ P1.0 */
#define MS51_CH0_ADC_CHANNEL   MS51_ADC_CH0
#define MS51_CH0_LED_MASK      0x01U   /* P0.0 */
#define MS51_CH0_DIP_MASK      0x01U   /* P1.0 */

/* Channel 2: ADC_CH1 @ P3.0, LED P0.1, DIP NC @ P1.1 */
#define MS51_CH1_ADC_CHANNEL   MS51_ADC_CH1
#define MS51_CH1_LED_MASK      0x02U   /* P0.1 */
#define MS51_CH1_DIP_MASK      0x02U   /* P1.1 */

#define MS51_FAULT_LED_MASK    0x04U   /* P0.2 */

void ms51_clock_init(void);
void ms51_gpio_init(void);
void ms51_adc_init(void);
uint16_t ms51_adc_read_averaged(uint8_t adc_channel, uint8_t sample_count);
void ms51_delay_ms(uint16_t ms);

#endif /* PLATFORM_MS51_H */
