#ifndef SC_CLK_H
#define SC_CLK_H

#include <systemc.h>
#include "sc_config.h"
#include "sc_run.h"

SC_MODULE (scClk) {
    // ---------------------      Ports      ---------------------
    sc_out<bool> clk{"clk_o"};

    SC_CTOR(scClk) {
        SC_THREAD(clockThread);
    }

    void clockThread () {
        sc_time ustep = sc_getMicrostep();
        int semiperiod_usteps = sc_getClkSemiperiodMicrosteps();
        bool act_edge = sc_getClkActEdge();
        while (1) {
            clk.write(act_edge);
            for (int i = 0; i < semiperiod_usteps; i++) {
                wait(ustep);
            }
            clk.write(!act_edge);
            for (int i = 0; i < semiperiod_usteps; i++) {
                wait(ustep);
            }
        }
    }

};

#endif // SC_CLK_H
