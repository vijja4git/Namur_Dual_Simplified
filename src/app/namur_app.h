#ifndef NAMUR_APP_H
#define NAMUR_APP_H

#include "namur_types.h"

/** One 20 ms update: both channels, LEDs, fault LEDs (matches production loop). */
void namur_app_tick(namur_system_state_t *state);

#endif /* NAMUR_APP_H */
