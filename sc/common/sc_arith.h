#ifndef SC_ARITH_H
#define SC_ARITH_H

#include <systemc.h>
#include "sc_gates.h"
#include "sc_gates_pv.h"

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscAdd : public SyscLogicGate<N,W> {
public:
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscAdd<N,W> SC_CURRENT_USER_MODULE;
    SyscAdd(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result = (sc_uint<W>)result + (sc_uint<W>)BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscAdd_pv : public SyscLogicGate_pv<N,W> {
public:
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscAdd_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscAdd_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            result += BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

// @remark The first input is the minuend; all the others add up for the subtrahend
template <unsigned int N = 2,
          unsigned int W = 1>
class SyscSub : public SyscLogicGate<N,W> {
public:
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscSub<N,W> SC_CURRENT_USER_MODULE;
    SyscSub(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result -= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

// @remark The first input is the minuend; all the others add up for the subtrahend
template <unsigned int N = 2,
          unsigned int W = 1>
class SyscSub_pv : public SyscLogicGate_pv<N,W> {
public:
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscSub_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscSub_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            result -= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};




#endif // SC_ARITH_H
