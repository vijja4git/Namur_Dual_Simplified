#ifndef NAMUR_LOGIC_H
#define NAMUR_LOGIC_H

#include <stdbool.h>
#include <stdint.h>

#include "namur_types.h"

uint16_t namur_adc_to_voltage_mv(uint16_t adc_avg);
uint16_t namur_adc_to_current_ua(uint16_t adc_avg);

void namur_logic_evaluate_channel(uint16_t adc_avg,
                                  bool dip_nc,
                                  bool prev_latch_on,
                                  namur_channel_state_t *state_out);

void namur_logic_init(namur_system_state_t *state);

#endif /* NAMUR_LOGIC_H */
