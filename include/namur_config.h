#ifndef NAMUR_CONFIG_H
#define NAMUR_CONFIG_H

/*
 * Thresholds calibrated to measured sensor levels (see docs/SENSOR_STATUS_GUIDE.md):
 *   Open:     0 V,     0 mA
 *   Idle:     0.67 V,  3.36 mA  (connected, object not detected)
 *   Sensing:  0.2-0.8 V, 0.3-1 mA (object detected)
 *   Short:    1.55 V,  8 mA
 */

#define NAMUR_NUM_CHANNELS        2U
#define NAMUR_LOOP_PERIOD_MS      20U
#define NAMUR_ADC_SAMPLE_COUNT    8U
#define NAMUR_ADC_RESOLUTION      4096U
#define NAMUR_VREF_MV             5000U
#define NAMUR_SHUNT_OHM           200U   /* Effective shunt (~V/I from 0.67V/3.36mA) */

/* Open / no sensor — 0 V, 0 mA */
#define NAMUR_V_OPEN_MV           80U
#define NAMUR_I_OPEN_UA           80U

/* Short circuit — 1.55 V, 8 mA */
#define NAMUR_V_SHORT_MV          1300U
#define NAMUR_I_SHORT_UA          7000U

/* Sensing ON (object detected) — 0.2-0.8 V, 0.3-1 mA */
#define NAMUR_V_SENSE_MIN_MV      180U
#define NAMUR_V_SENSE_MAX_MV      850U
#define NAMUR_I_SENSE_MIN_UA      250U
#define NAMUR_I_SENSE_MAX_UA      1100U

/* Connected idle (not detecting) — ~0.67 V, ~3.36 mA → latch OFF */
#define NAMUR_I_IDLE_MIN_UA       2800U

/* Hysteresis hold band between sense max and idle min: 1101-2799 µA */

#endif /* NAMUR_CONFIG_H */
