#ifndef NAMUR_CONFIG_H
#define NAMUR_CONFIG_H

/* Application constants — MCU-independent */

#define NAMUR_NUM_CHANNELS        2U
#define NAMUR_LOOP_PERIOD_MS      20U
#define NAMUR_ADC_SAMPLE_COUNT    8U
#define NAMUR_ADC_RESOLUTION      4096U   /* 12-bit: 0..4095 */
#define NAMUR_VREF_MV             5000U   /* 5.0 V reference */
#define NAMUR_SHUNT_MILLIOHM      500000U /* 500 ohm expressed as milliohm */

/* Thresholds in microamps (µA) */
#define NAMUR_THRESH_FAULT_OPEN_UA    150U
#define NAMUR_THRESH_FAULT_SHORT_UA   6000U
#define NAMUR_THRESH_ON_UA            2100U
#define NAMUR_THRESH_OFF_UA            1200U

#endif /* NAMUR_CONFIG_H */
