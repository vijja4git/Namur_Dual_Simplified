#ifndef NAMUR_CONFIG_H
#define NAMUR_CONFIG_H

/*
 * Application constants — MCU-independent.
 * Change thresholds here; then rebuild firmware and re-read docs/HOW_THE_DEVICE_WORKS.md.
 */

#define NAMUR_NUM_CHANNELS        2U    /* Two independent NAMUR loops */
#define NAMUR_LOOP_PERIOD_MS      20U   /* Main loop sleeps 20 ms between updates */
#define NAMUR_ADC_SAMPLE_COUNT    8U    /* BSP averages this many ADC readings per channel */
#define NAMUR_ADC_RESOLUTION      4096U /* 12-bit ADC: counts 0..4095, divide by 4096 in math */
#define NAMUR_VREF_MV             5000U /* ADC reference 5.0 V (documented assumption) */
#define NAMUR_SHUNT_MILLIOHM      500000U /* Shunt resistor 500 Ω (stored as milliohm for docs) */

/* Loop current thresholds in microamps (µA) — see namur_logic.c */
#define NAMUR_THRESH_FAULT_OPEN_UA    150U   /* Below → wire break / open circuit fault */
#define NAMUR_THRESH_FAULT_SHORT_UA   6000U  /* Above → short circuit fault */
#define NAMUR_THRESH_ON_UA            2100U  /* Above → latch ON (normal band) */
#define NAMUR_THRESH_OFF_UA            1200U  /* Below → latch OFF; between OFF and ON = hold */

#endif /* NAMUR_CONFIG_H */
