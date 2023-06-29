#include "sc_config.h"

static sc_thread_config_t *scThreadConfig = nullptr;

void sc_setConfig(sc_thread_config_t *theConfig)
{
    scThreadConfig = theConfig;
}

bool sc_getRstActLevel(void)
{
    if (scThreadConfig == nullptr) {
        return false;
    }
    return scThreadConfig->rst_act_level;
}

int sc_getRstActMicrosteps(void)
{
    if (scThreadConfig == nullptr) {
        return 0;
    }
    return scThreadConfig->rst_act_microsteps;
}

bool sc_getEnaActLevel(void)
{
    if (scThreadConfig == nullptr) {
        return false;
    }
    return scThreadConfig->ena_act_level;
}

bool sc_getClkActEdge(void)
{
    if (scThreadConfig == nullptr) {
        return false;
    }
    return scThreadConfig->clk_act_edge;
}

sc_time sc_getClkPeriod(void)
{
    if (scThreadConfig == nullptr) {
        return sc_time(0, SC_NS);
    }
    return scThreadConfig->clk_period;
}

sc_time sc_getMicrostep(void)
{
    if (scThreadConfig == nullptr) {
        return sc_time(0, SC_NS);
    }
    return scThreadConfig->microstep;
}

int sc_getClkSemiperiodMicrosteps(void)
{
    if (scThreadConfig == nullptr) {
        return 0;
    }
    return scThreadConfig->clk_semiperiod_microsteps;
}

