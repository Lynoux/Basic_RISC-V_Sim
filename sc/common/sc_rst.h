#ifndef SC_RST_H
#define SC_RST_H

#include <systemc.h>
#include "sc_config.h"

SC_MODULE (scRst) {
    sc_out<bool> rst_o{"rst_o"};

    SC_CTOR(scRst) {
        SC_THREAD(resetThread);
    }

    void resetThread () {
        sc_time ustep = sc_getMicrostep();
        bool act_level = sc_getRstActLevel();
        int act_usteps  =sc_getRstActMicrosteps();
        while (1) {
            rst_o.write(act_level);
            for (int i = 0; i < act_usteps; i++) {
                wait(ustep);
            }
            rst_o.write(!act_level);
            for (;;) {
                wait(ustep);
            }
        }
    }

};


#endif // SC_RST_H
