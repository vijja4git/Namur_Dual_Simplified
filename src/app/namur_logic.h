#ifndef NAMUR_LOGIC_H
#define NAMUR_LOGIC_H

#include <stdbool.h>
#include <stdint.h>

#include "namur_types.h"

/** Convert averaged 12-bit ADC count to loop current in microamps. */
uint16_t namur_adc_to_current_ua(uint16_t adc_avg);

/**
 * Evaluate one channel from measured current and DIP mode.
 * prev_latch_on is the latched state from the previous cycle.
 */
void namur_logic_evaluate_channel(uint16_t current_ua,
                                  bool dip_nc,
                                  bool prev_latch_on,
                                  namur_channel_state_t *state_out);

/** True if any channel reports a fault (for global fault LED). */
bool namur_logic_any_fault(const namur_system_state_t *state);

void namur_logic_init(namur_system_state_t *state);

#endif /* NAMUR_LOGIC_H */
