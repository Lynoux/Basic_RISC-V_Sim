#ifndef SC_TRI_H
#define SC_TRI_H

#include <iostream>
#include <string>
#include <systemc.h>
#include "sc_gates.h"

/**
 * Numeric conversion to use when selection value contains X or Z
 */
typedef enum TRI_CONV_CTL_XORZ_e {
    TRI_CONV_CTL_XORZ_X,  /**< Tri output is X (default)         */
    TRI_CONV_CTL_XORZ_Z,  /**< Tri output is Z (assume ctl == 0) */
    TRI_CONV_CTL_XORZ_D   /**< Tri output is D (assume ctl == 1) */
} TRI_CONV_CTL_XORZ_t;

template <unsigned int W = 1, TRI_CONV_CTL_XORZ_t TRI_XORZ_CONV = TRI_CONV_CTL_XORZ_X>
class SyscTri : public SyscLogicGate<1,W> {
public:
    static_assert(W >= 1, "SyscTri DESIGN ERROR: W must be >= 1");
    typedef  sc_lv<1>       ctl_t;
    sc_in<ctl_t>            ctl{"ctl"}; // Control input (0/1)

    typedef SyscLogicGate<1,W> BASE_MODULE;
    typedef SyscTri<W,TRI_XORZ_CONV> SC_CURRENT_USER_MODULE;
    SyscTri(::sc_core::sc_module_name name) : SyscLogicGate<1,W>(name) {
        BASE_MODULE::sensitive << ctl;
    }

    static inline const sc_lv<1> y_is_d{::sc_dt::SC_LOGIC_1};
    static inline const typename BASE_MODULE::data_t allx{::sc_dt::SC_LOGIC_X};
    static inline const typename BASE_MODULE::data_t allz{::sc_dt::SC_LOGIC_Z};

    virtual void combinational () {
        if (ctl.read().is_01()) {
            if (ctl.read() == y_is_d) {
                BASE_MODULE::y.write(BASE_MODULE::d.read());
            } else {
                BASE_MODULE::y.write(allz);
            }
        } else {
            switch (TRI_XORZ_CONV) {
            case TRI_CONV_CTL_XORZ_X:
                BASE_MODULE::y.write(allx);
                break;
            case TRI_CONV_CTL_XORZ_Z:
                BASE_MODULE::y.write(allz);
                break;
            default: // TRI_CONV_CTL_XORZ_D
                BASE_MODULE::y.write(BASE_MODULE::d.read());
            }
        }
    }
};

#endif // SC_TRI_H
