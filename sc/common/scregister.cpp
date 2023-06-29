#include "scregister.h"

scRegister::scRegister (::sc_core::sc_module_name) {
    SC_THREAD(store);
    rst_level = sc_getRstActLevel();
    ena_level = sc_getEnaActLevel();
    sensitive << reset;
    if (sc_getClkActEdge()) {
        sensitive << clock.pos();
    } else {
        sensitive << clock.neg();
    }
}

void scRegister::store () {
    while(1) {
        // At every rising edge of clock we check if reset is active
        if (reset.read() == rst_level) {
            // If reset is active, set internal value to 00...0
            q.write(0);
        } else if (enable.read() == ena_level) {
            // If reset not active, check if enable is active to store a new value
            q.write(d.read());
        }
        wait();
    }
}

