#ifndef SC_MUX_H
#define SC_MUX_H

#include <systemc.h>
#include "sc_gates.h"
#include "sc_gates_pv.h"

#if 0
template <typename T, unsigned int N=2>
class scMuxT : public ::sc_core::sc_module {
public:
    static_assert(N >= 2, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<T>  d[N];                  // WIDTH bit x N data inputs
    sc_in<unsigned int>  sel{"sel"}; // Selection input (from 0 to N-1)
    sc_out<T> y{"y"};                // WIDTH bit data output

    SC_CTOR(scMuxT) {
        SC_METHOD(combinational);
        for (unsigned int i=0; i < N; i++) {
            sensitive << d[i];
        }
        sensitive << sel;
    }

    void combinational () {
        unsigned int selection = sel.read();
        if (selection >= N) {
            y.write(0);
        } else {
            y.write(d[selection].read());
        }
    }
};


template <unsigned int N=8>
class mux2to1 : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  data0;
    sc_in<sc_lv<N>>  data1;
    sc_in<bool>  sel{"sel"}; // Selection input (from 0 or 1)
    sc_out<sc_lv<N>>  result;

    SC_CTOR(mux2to1) {
        SC_METHOD(combinational);
        sensitive << data0;
        sensitive << data1;
        sensitive << sel;
    }

    void combinational () {
        if (sel.read()) {
            result.write(data0.read());
        } else {
            result.write(data1.read());
        }
    }
};
#endif

/**
 * Numeric conversion to use when selection value contains X or Z
 */
typedef enum MUX_CONV_SEL_XORZ_e {
    MUX_CONV_SEL_XORZ_FIRST,  /**< Assume selection = 0 (first data input, default) */
    MUX_CONV_SEL_XORZ_LAST,   /**< Assume selection = N-1 (last data input)         */
    MUX_CONV_SEL_XORZ_SYSTEMC /**< SystemC's default conversion (issues warning)    */
} MUX_CONV_SEL_XORZ_t;

/**
 * Mux behaviour for invalid selection input value
 */
typedef enum MUX_SEL_INVALID_BEH_e {
    MUX_BEH_SEL_INVALID_ZEROES, /**< Set output to all zeroes (default) */
    MUX_BEH_SEL_INVALID_ONES,   /**< Set output to all ones             */
    MUX_BEH_SEL_INVALID_FIRST,  /**< Set output to first data input     */
    MUX_BEH_SEL_INVALID_LAST    /**< Set output to last data input      */
} MUX_SEL_INVALID_BEH_t;


#define MUX_DEPENDS_ON_GATES 1

#ifdef MUX_DEPENDS_ON_GATES


template <unsigned int N = 2,
          unsigned int W = 1, unsigned int SELW = 1,
          MUX_SEL_INVALID_BEH_t SEL_INVALID_BEH = MUX_BEH_SEL_INVALID_ZEROES,
          MUX_CONV_SEL_XORZ_t   SEL_XORZ_CONV = MUX_CONV_SEL_XORZ_FIRST>
class SyscMux : public SyscLogicGate<N,W> {
public:
    static inline const unsigned int log2_N_ = static_cast<int>(ceil(log(N)));
    static_assert(SELW >= 1, "SyscMuxT DESIGN ERROR: SELW must be >=1");
    static_assert(SELW >= log2_N_, "SyscMuxT DESIGN ERROR: SELW must be >= log_2(N)");

    typedef  sc_lv<SELW>       sel_t;
    sc_in<sel_t>  sel{"sel"}; // Selection input (from 0 to N-1)

    typedef SyscLogicGate<N,W> BASE_MODULE;
    typedef typename BASE_MODULE::data_t data_t;
    typedef SyscMux<N,W,SELW,SEL_INVALID_BEH,SEL_XORZ_CONV> SC_CURRENT_USER_MODULE;
    static inline const data_t all0{::sc_dt::SC_LOGIC_0};
    static inline const data_t all1{::sc_dt::SC_LOGIC_1};
    SyscMux(::sc_core::sc_module_name name) : SyscLogicGate<N,W>(name) {
        sc_module::sensitive << sel;
    }

    virtual void combinational () {
        std::string sel_value(sel.read().to_string());
        unsigned int temp = 0;
        if (sel_value.find_first_of("xXzZ") == std::string::npos) {
            temp = static_cast<sc_uint<log2_N_>>(sel->read());
        } else {
            switch(SEL_XORZ_CONV) {
            case MUX_CONV_SEL_XORZ_LAST:
                temp = N - 1;
                break;
            case MUX_CONV_SEL_XORZ_SYSTEMC:
                temp = static_cast<sc_uint<log2_N_>>(sel->read());
                break;
            default: // MUX_CONV_SEL_XORZ_FIRST:
                temp = 0;
            }
        }
        if (temp < N) {
            BASE_MODULE::y.write(BASE_MODULE::d[temp]->read());
        } else {
            switch (SEL_INVALID_BEH) {
            case MUX_BEH_SEL_INVALID_ONES:
                BASE_MODULE::y.write(all1);
                break;
            case MUX_BEH_SEL_INVALID_FIRST:
                BASE_MODULE::y.write(BASE_MODULE::d[0]->read());
                break;
            case MUX_BEH_SEL_INVALID_LAST:
                BASE_MODULE::y.write(BASE_MODULE::d[N-1]->read());
                break;
            default: // MUX_BEH_SEL_INVALID_ZEROES
                BASE_MODULE::y.write(all0);
            }
        }
    }
};

template <unsigned int N = 2,
          unsigned int W = 1, unsigned int SELW = 1,
          MUX_SEL_INVALID_BEH_t SEL_INVALID_BEH = MUX_BEH_SEL_INVALID_ZEROES,
          MUX_CONV_SEL_XORZ_t   SEL_XORZ_CONV = MUX_CONV_SEL_XORZ_FIRST>
class SyscMux_pv : public SyscLogicGate_pv<N,W> {
public:
    static inline const unsigned int log2_N_ = static_cast<int>(ceil(log(N)));
    static_assert(SELW >= 1, "SyscMuxT DESIGN ERROR: SELW must be >=1");
    static_assert(SELW >= log2_N_, "SyscMuxT DESIGN ERROR: SELW must be >= log_2(N)");

    typedef  sc_lv<SELW>       sel_t;
    sc_in<sel_t>  sel{"sel"}; // Selection input (from 0 to N-1)

    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef typename BASE_MODULE::data_t data_t;
    typedef SyscMux_pv<N,W,SELW,SEL_INVALID_BEH,SEL_XORZ_CONV> SC_CURRENT_USER_MODULE;
    static inline const data_t all0{::sc_dt::SC_LOGIC_0};
    static inline const data_t all1{::sc_dt::SC_LOGIC_1};
    SyscMux_pv(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
        sc_module::sensitive << sel;
    }

    virtual void combinational () {
        std::string sel_value(sel.read().to_string());
        unsigned int temp = 0;
        if (sel_value.find_first_of("xXzZ") == std::string::npos) {
            temp = static_cast<sc_uint<log2_N_>>(sel->read());
        } else {
            switch(SEL_XORZ_CONV) {
            case MUX_CONV_SEL_XORZ_LAST:
                temp = N - 1;
                break;
            case MUX_CONV_SEL_XORZ_SYSTEMC:
                temp = static_cast<sc_uint<log2_N_>>(sel->read());
                break;
            default: // MUX_CONV_SEL_XORZ_FIRST:
                temp = 0;
            }
        }
        if (temp < N) {
            BASE_MODULE::y.write(BASE_MODULE::d[temp]->read());
        } else {
            switch (SEL_INVALID_BEH) {
            case MUX_BEH_SEL_INVALID_ONES:
                BASE_MODULE::y.write(all1);
                break;
            case MUX_BEH_SEL_INVALID_FIRST:
                BASE_MODULE::y.write(BASE_MODULE::d[0]->read());
                break;
            case MUX_BEH_SEL_INVALID_LAST:
                BASE_MODULE::y.write(BASE_MODULE::d[N-1]->read());
                break;
            default: // MUX_BEH_SEL_INVALID_ZEROES
                BASE_MODULE::y.write(all0);
            }
        }
    }
};



#else



template <unsigned int N = 2,
          unsigned int W = 1,
          MUX_SEL_INVALID_BEH_t SEL_INVALID_BEH = MUX_BEH_SEL_INVALID_ZEROES,
          MUX_CONV_SEL_XORZ_t   SEL_XORZ_CONV = MUX_CONV_SEL_XORZ_FIRST>
class SyscMuxT : public ::sc_core::sc_module {
public:
    static_assert(N >= 2, "SyscMuxT DESIGN ERROR: N must be >=2");
    static_assert(W >= 1, "SyscMuxT DESIGN ERROR: W must be >=1");

    static inline const unsigned int log2_N_ = static_cast<int>(ceil(log(N)));
    typedef  sc_lv<W>       data_t;
    typedef  sc_lv<log2_N_> sel_t;

    sc_in<data_t>          *d[N];       // Data input
                                        // Array of pointers so each port name can be initialized in the constructor to 'd0', 'd1', and so on (instead of 'port_0', 'port_1', etc.)
                                        // mapping is done as (*mux_object.d[index])(signal_to_bind_the_port) instead of mux_object.d[index](signal_to_bind_the_port)
                                        // see https://stackoverflow.com/questions/35425052/how-to-initialize-a-systemc-port-name-which-is-an-array/35535730#35535730
    sc_in<sel_t>            sel{"sel"}; // Selection input [0, N-1]
    sc_out<data_t>          y{"y"};     // Data output

    static inline const data_t all0{::sc_dt::SC_LOGIC_0};
    static inline const data_t all1{::sc_dt::SC_LOGIC_1};

    SC_CTOR(SyscMuxT) {
        SC_METHOD(combinational);
        for (unsigned int i=0; i < N; i++) {
            d[i] = new sc_in<data_t>(("d" + std::to_string(i)).c_str());
            sensitive << *d[i];
        }
        sensitive << sel;
    }

    void combinational () {
        std::string sel_value(sel.read().to_string());
        unsigned int temp = 0;
        if (sel_value.find_first_of("xXzZ") == std::string::npos) {
            temp = static_cast<sc_uint<log2_N_>>(sel->read());
        } else {
            switch(SEL_XORZ_CONV) {
            case MUX_CONV_SEL_XORZ_LAST:
                temp = N - 1;
                break;
            case MUX_CONV_SEL_XORZ_SYSTEMC:
                temp = static_cast<sc_uint<log2_N_>>(sel->read());
                break;
            default: // MUX_CONV_SEL_XORZ_FIRST:
                temp = 0;
            }
        }
        if (temp < N) {
            y.write(d[temp]->read());
        } else {
            switch (SEL_INVALID_BEH) {
            case MUX_BEH_SEL_INVALID_ONES:
                y.write(all1);
                break;
            case MUX_BEH_SEL_INVALID_FIRST:
                y.write(d[0]->read());
                break;
            case MUX_BEH_SEL_INVALID_LAST:
                y.write(d[N-1]->read());
                break;
            default: // MUX_BEH_SEL_INVALID_ZEROES
                y.write(all0);
            }
        }
    }
};

template <unsigned int N = 2,
          unsigned int W = 1,
          MUX_SEL_INVALID_BEH_t SEL_INVALID_BEH = MUX_BEH_SEL_INVALID_ZEROES,
          MUX_CONV_SEL_XORZ_t   SEL_XORZ_CONV = MUX_CONV_SEL_XORZ_FIRST>
class SyscMuxT_pv : public ::sc_core::sc_module {
public:
    static_assert(N >= 2, "SyscMuxT DESIGN ERROR: N must be >=2");
    static_assert(W >= 1, "SyscMuxT DESIGN ERROR: W must be >=1");

    static inline const unsigned int log2_N_ = static_cast<int>(ceil(log(N)));
    typedef  sc_lv<W>       data_t;
    typedef  sc_lv<log2_N_> sel_t;

    sc_vector<sc_in<data_t>> d;         // Data input
                                        // sc_vector of ports; name can be initialized in the constructor to 'd_0', 'd_1', and so on (instead of 'port_0', 'port_1', etc.)
                                        // see https://stackoverflow.com/questions/35425052/how-to-initialize-a-systemc-port-name-which-is-an-array/35535730#35535730
    sc_in<sel_t>            sel{"sel"}; // Selection input [0, N-1]
    sc_out<data_t>          y{"y"};     // Data output

    static inline const data_t all0{::sc_dt::SC_LOGIC_0};
    static inline const data_t all1{::sc_dt::SC_LOGIC_1};

    SC_CTOR(SyscMuxT_pv) : d("d", N) {
        SC_METHOD(combinational);
        for (unsigned int i=0; i < N; i++) {
            sensitive << d[i];
        }
        sensitive << sel;
    }

    void combinational () {
        std::string sel_value(sel.read().to_string());
        unsigned int temp = 0;
        if (sel_value.find_first_of("xXzZ") == std::string::npos) {
            temp = static_cast<sc_uint<log2_N_>>(sel->read());
        } else {
            switch(SEL_XORZ_CONV) {
            case MUX_CONV_SEL_XORZ_LAST:
                temp = N - 1;
                break;
            case MUX_CONV_SEL_XORZ_SYSTEMC:
                temp = static_cast<sc_uint<log2_N_>>(sel->read());
                break;
            default: // MUX_CONV_SEL_XORZ_FIRST:
                temp = 0;
            }
        }
        if (temp < N) {
            y.write(d[temp].read());
        } else {
            switch (SEL_INVALID_BEH) {
            case MUX_BEH_SEL_INVALID_ONES:
                y.write(all1);
                break;
            case MUX_BEH_SEL_INVALID_FIRST:
                y.write(d[0].read());
                break;
            case MUX_BEH_SEL_INVALID_LAST:
                y.write(d[N-1].read());
                break;
            default: // MUX_BEH_SEL_INVALID_ZEROES
                y.write(all0);
            }
        }
    }
};
#endif

#endif // SC_MUX_H
