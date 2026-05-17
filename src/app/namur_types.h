#ifndef NAMUR_TYPES_H
#define NAMUR_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    NAMUR_FAULT_NONE = 0,
    NAMUR_FAULT_LEAD_BREAK,
    NAMUR_FAULT_SHORT_CIRCUIT
} namur_fault_t;

typedef struct {
    uint16_t      adc_avg;
    uint16_t      voltage_mv;
    uint16_t      current_ua;
    namur_fault_t fault;
    bool          latch_on;
    bool          output_on;
    bool          channel_led_on;
    bool          fault_led_on;
} namur_channel_state_t;

typedef struct {
    namur_channel_state_t channel[2];
} namur_system_state_t;

#endif /* NAMUR_TYPES_H */
