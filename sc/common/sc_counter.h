#ifndef SC_COUNTER_H
#define SC_COUNTER_H

#include <systemc.h>
#include "sc_config.h"
#include "sc_reg.h"
#include "sc_flexint.h"

SC_MODULE (scCounter) {
    // ---------------------      Ports      ---------------------
    sc_in_clk           clock{"clock"};         // Falling-edge active clock input
    sc_in<bool>         reset_i{"reset_i"};     // Active-high synchronous reset input
    sc_in<bool>         enable_i{"enable_i"};   // Active-high enable input
    sc_out<sc_lv<32>>   q_o{"q_o"};             // 32 bit data output

    // --------------------- Local variables ---------------------
    bool                rst_level;              // Module parameter (aka "VHDL generic")
    bool                ena_level;              // Module parameter (aka "VHDL generic")

    // Constructor for the scRegister
    SC_CTOR(scCounter) {
        SC_THREAD(doCount);
        rst_level = sc_getRstActLevel();
        ena_level = sc_getEnaActLevel();
        sensitive << reset_i;
        if (sc_getClkActEdge()) {
            sensitive << clock.pos();
        } else {
            sensitive << clock.neg();
        }
    }

    // Actual scCounter logic
    void doCount () {
        while(1) {
            if (reset_i.read() == rst_level) {
                q_o.write(0);
            } else if (enable_i.read() == ena_level) {
                q_o.write(q_o.read().to_int64()+1);
            }
            wait();
        }
    }
};


SC_FLEXINT_MODULE (scFlexCounter) {
public:
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<bool>       reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<bool>       enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<sc_lv<32>>   q_o{"q_o"};             // 32 bit data output

    // Constructor for the scRegister
    SC_CTOR(scFlexCounter) {
        SC_THREAD(doCount);
        this->doSensitive();
    }

    // Actual scCounter logic
    void doCount () {
        while(1) {
            if (this->bResetAction()) {
                q_o.write(0);
            } else if (this->bEnableAction()) {
                q_o.write(q_o.read().to_int64()+1);
            }
            wait();
        }
    }
};


SC_FLEXINT_MODULE_T (scFlexCounterT,typename T) {
public:
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<bool>       reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<bool>       enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<T>            q_o{"q_o"};        // Counter output

    // Constructor for the scRegister
    SC_CTOR(scFlexCounterT) {
        SC_THREAD(doCount);
        this->doSensitive();
    }

    // Actual scCounter logic
    void doCount () {
        while(1) {
            if (this->bResetAction()) {
                q_o.write(0);
            } else if (this->bEnableAction()) {
                if constexpr(is_boolean<T>::value) {
                    // Boolean types don't have members like is_01() / read(), can't add
                    q_o.write(!q_o.read());
                    // cout << "@" << sc_time_stamp() << "scFlexCounterT() <bool> updated " << !q_o.read() << endl;
                } else if constexpr(std::is_integral<T>::value) {
                    // Integral types don't have members like is_01() / read()
                    q_o.write(q_o.read() + 1);
                    cout << "@" << sc_time_stamp() << "scFlexCounterT() <integral> updated " << q_o.read()+1 << endl;
                } else if constexpr(std::is_floating_point<T>::value) {
                    // Floating-point types don't have members like is_01() / read()
                    q_o.write(q_o.read() + 1.0);
                    // cout << "@" << sc_time_stamp() << "scFlexCounterT() <FP> updated " << q_o.read()+1.0 << endl;
                } else {
                    // Other datatypes do have members like is_01() / read()
                    if (q_o.read().is_01()) {
                        q_o.write(q_o.read().to_uint64() + 1);
                        // cout << "@" << sc_time_stamp() << "scFlexCounterT() <T> updated, is_01() " << q_o.read().to_uint64()+1.0 << endl;
                    } else {
                        q_o.write(q_o.read());
                        // cout << "@" << sc_time_stamp() << "scFlexCounterT() <T> updated, !is_01() " << q_o.read() << endl;
                    }
                }
            }
            wait();
        }
    }
};




SC_MODULE (scCounterStructural) {
    // ---------------------      Ports      ---------------------
    sc_in_clk           clock{"clock"};         // Falling-edge active clock input
    sc_in<bool>         reset_i{"reset_i"};     // Active-high synchronous reset input
    sc_in<bool>         enable_i{"enable_i"};   // Active-high enable input
    sc_out<sc_lv<32>>   q_o{"q_o"};             // 32 bit data output
    // --------------------- Intenal signals ---------------------
    sc_signal<sc_lv<32>> q_i{"q_i"};
    // --------------------- Internal blocks ---------------------
    scFlexRegisterT<sc_lv<32>> reg {"REG"};

    SC_CTOR(scCounterStructural) {
        SC_THREAD(structure);
        sensitive << q_o;
        reg.clock(clock);
        reg.reset(reset_i);
        reg.enable(enable_i);
        reg.d(q_i);
        reg.q(q_o);
    }

    void structure() {
        while (1) {
            if (reg.q.read().is_01()) {
                q_i.write(q_o.read().to_uint64() + 1);
            } else {
                q_i.write(q_o.read());
            }
            wait();
        }
    }
};


SC_FLEXINT_MODULE (scFlexCounterStructural) {
public:
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<bool>       reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<bool>       enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<sc_lv<32>>    q_o{"q_o"};        // 32 bit data output
    // --------------------- Intenal signals ---------------------
    sc_signal<sc_lv<32>> q_i{"q_i"};
    // --------------------- Internal blocks ---------------------
    scFlexRegisterT<sc_lv<32>> reg {"REG"};

    // Constructor for the scRegister
    SC_CTOR(scFlexCounterStructural) {
        SC_THREAD(structure);
        this->doSensitive();
        reg.clock(this->clock);
        reg.reset(this->reset);
        reg.enable(this->enable);
        reg.d(this->q_i);
        reg.q(this->q_o);
    }

    void structure() {
        while (1) {
            if (reg.q.read().is_01()) {
                q_i.write(q_o.read().to_uint64() + 1);
            } else {
                q_i.write(q_o.read());
            }
            wait();
        }
    }
};


SC_FLEXINT_MODULE_T (scFlexCounterStructuralT,typename T) {
public:
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<bool>       reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<bool>       enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<T>            q_o{"q_o"};        // 32 bit data output
    // --------------------- Intenal signals ---------------------
    sc_signal<T>         q_i{"q_i"};
    // --------------------- Internal blocks ---------------------
    scFlexRegisterT<T>   reg {"REG"};

    // Constructor for the scRegister
    SC_CTOR(scFlexCounterStructuralT) {
        SC_THREAD(structure);
        this->doSensitive();
        reg.clock(this->clock);
        reg.reset(this->reset);
        reg.enable(this->enable);
        reg.d(this->q_i);
        reg.q(this->q_o);
    }

    void structure() {
        while (1) {
            if constexpr(is_boolean<T>::value) {
                // Boolean types don't have members like is_01() / read(), can't add
                q_i.write(!q_o.read());
                // cout << "@" << sc_time_stamp() << "scFlexCounterStructuralT() <bool> updated " << !q_o.read() << endl;
            } else if constexpr(std::is_integral<T>::value) {
                // Integral types don't have members like is_01() / read()
                q_i.write(q_o.read() + 1);
                cout << "@" << sc_time_stamp() << "scFlexCounterStructuralT() <integral> updated " << q_o.read()+1 << endl;
            } else if constexpr(std::is_floating_point<T>::value) {
                // Floating-point types don't have members like is_01() / read()
                q_i.write(q_o.read() + 1.0);
                // cout << "@" << sc_time_stamp() << "scFlexCounterStructuralT() <FP> updated " << q_o.read()+1.0 << endl;
            } else {
                // Other datatypes do have members like is_01() / read()
                if (reg.q.read().is_01()) {
                    q_i.write(q_o.read().to_uint64() + 1);
                    // cout << "@" << sc_time_stamp() << "scFlexCounterStructuralT() <T> updated, is_01() " << q_o.read().to_uint64()+1.0 << endl;
                } else {
                    q_i.write(q_o.read());
                    // cout << "@" << sc_time_stamp() << "scFlexCounterStructuralT() <T> updated, !is_01() " << q_o.read() << endl;
                }
            }
            wait();
        }
    }
};

#endif // SC_COUNTER_H
