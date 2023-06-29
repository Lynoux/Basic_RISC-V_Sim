#ifndef SC_GATES_H
#define SC_GATES_H

#include <systemc.h>

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscLogicGate : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "SyscLogicGate DESIGN ERROR: N must be >= 1");
    static_assert(W >= 1, "SyscLogicGate DESIGN ERROR: W must be >= 1");

    typedef  sc_lv<W>       data_t;

    sc_in<data_t>          *d[N];       // Data input(s)
                                        // Array of pointers so each port name can be initialized in the constructor to 'd0', 'd1', and so on (instead of 'port_0', 'port_1', etc.)
                                        // mapping is done as (*mux_object.d[index])(signal_to_bind_the_port) instead of mux_object.d[index](signal_to_bind_the_port)
                                        // see https://stackoverflow.com/questions/35425052/how-to-initialize-a-systemc-port-name-which-is-an-array/35535730#35535730
    sc_out<data_t>          y{"y"};     // Data output

    typedef SyscLogicGate<N,W> SC_CURRENT_USER_MODULE;
    SyscLogicGate(::sc_core::sc_module_name name) : ::sc_core::sc_module(name) {
        SC_METHOD(combinational);
        if (1 == N) {
            d[0] = new sc_in<data_t>("d");
            sensitive << *d[0];
        } else {
            for (unsigned int i=0; i < N; i++) {
                d[i] = new sc_in<data_t>(("d" + std::to_string(i)).c_str());
                sensitive << *d[i];
            }
        }
    }

    virtual void combinational () {
        data_t result(SC_LOGIC_X);
        y.write(result);
    }
};





template <unsigned int N = 2,
          unsigned int W = 1>
class SyscAnd : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscAnd DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscAnd DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscAnd<N,W> SC_CURRENT_USER_MODULE;
    SyscAnd(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result &= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscNand : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscNand DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscNand DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscNand<N,W> SC_CURRENT_USER_MODULE;
    SyscNand(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result &= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(~result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscOr : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscOr DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscOr DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscOr<N,W> SC_CURRENT_USER_MODULE;
    SyscOr(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result |= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscNor : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscNor DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscNor DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscNor<N,W> SC_CURRENT_USER_MODULE;
    SyscNor(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result |= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(~result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscXor : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscXor DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscXor DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscXor<N,W> SC_CURRENT_USER_MODULE;
    SyscXor(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result ^= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscXnor : public SyscLogicGate<N,W> {
public:
    static_assert(N >= 2, "SyscXnor DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscXnor DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef SyscXnor<N,W> SC_CURRENT_USER_MODULE;
    SyscXnor(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
    }

    virtual void combinational () {
        typename BASE_MODULE::data_t result2;
        unsigned int i = N-1;
        auto result = BASE_MODULE::d[0]->read();
        for (; i > 0; i--) {
            result ^= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(~result);
    }
};

#endif // SC_GATES_H
