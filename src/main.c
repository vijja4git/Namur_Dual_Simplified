#include "namur_app.h"
#include "namur_config.h"
#include "namur_logic.h"
#include "platform.h"

static namur_system_state_t g_state;

int main(void)
{
    platform_init();
    namur_logic_init(&g_state);

    for (;;) {
        namur_app_tick(&g_state);
        platform_delay_ms(NAMUR_LOOP_PERIOD_MS);
    }
}
