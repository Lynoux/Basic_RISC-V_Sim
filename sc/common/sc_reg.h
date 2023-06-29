#ifndef SC_REG_H
#define SC_REG_H

#include <systemc.h>
#include "sc_flexint.h"


template <unsigned int W = 1,
          bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class SyscReg : public SyscFlexInt<CLKEDGE, FLEXINT> {

public:
    typedef  sc_lv<W>       data_t;
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<rst_t>      reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<ena_t>      enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_in<data_t>  d{"d"};                  // Data input
    sc_out<data_t> q{"q"};                  // Data output

    typedef SyscFlexInt<CLKEDGE, FLEXINT> BASE_MODULE;
    typedef SyscReg<W,CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;
    SyscReg(::sc_core::sc_module_name name) : SyscFlexInt<CLKEDGE, FLEXINT>(name) {
        SC_THREAD(react);
        this->doSensitive();
    }

    void react () {
        while(1) {
            if (this->bResetAction()) {
                q.write(0);
            } else if (this->bEnableAction()) {
                q.write(d.read());
            }
            wait();
        }
    }
};

template <unsigned int W = 1,
          bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class SyscCnt : public SyscFlexInt<CLKEDGE, FLEXINT> {

public:
    typedef  sc_lv<W>       data_t;
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<rst_t>      reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<ena_t>      enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<data_t> q{"q"};                  // Data output

    typedef SyscFlexInt<CLKEDGE, FLEXINT> BASE_MODULE;
    typedef SyscCnt<W,CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;
    SyscCnt(::sc_core::sc_module_name name) : SyscFlexInt<CLKEDGE, FLEXINT>(name) {
        SC_THREAD(react);
        this->doSensitive();
    }

    void react () {
        while(1) {
            if (this->bResetAction()) {
                q.write(0);
            } else if (this->bEnableAction()) {
                q.write((sc_uint<W>)q.read() + 1);
            }
            wait();
        }
    }
};


#endif // SC_REG_H
