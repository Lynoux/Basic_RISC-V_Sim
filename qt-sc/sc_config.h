#ifndef SC_CONFIG_H
#define SC_CONFIG_H

#include <systemc.h>


typedef struct sc_thread_config_s {
    bool rst_act_level;
    int rst_act_microsteps;
    bool ena_act_level;
    bool clk_act_edge;
    sc_time clk_period;
    sc_time microstep;
    int clk_semiperiod_microsteps;
} sc_thread_config_t;

void sc_setConfig(sc_thread_config_t *theConfig);

bool sc_getRstActLevel(void);
int sc_getRstActMicrosteps(void);
bool sc_getEnaActLevel(void);
bool sc_getClkActEdge(void);
sc_time sc_getClkPeriod(void);
sc_time sc_getMicrostep(void);
int sc_getClkSemiperiodMicrosteps(void);

#endif // SC_CONFIG_H
