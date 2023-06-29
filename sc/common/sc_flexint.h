#ifndef SC_FLEXINT_H
#define SC_FLEXINT_H

#include <stdint.h>
#include <systemc.h>

#define DEFAULT_CLKEDGE     true
#define DEFAULT_FLEXINT     RST_LOW_ENA_HIGH

typedef enum flexinterface_e {NO_RST_NO_ENA   = 0x00, NO_RST_ENA_HIGH   = 0x01, NO_RST_ENA_LOW   = 0x02,
                              RST_HIGH_NO_ENA = 0x10, RST_HIGH_ENA_HIGH = 0x11, RST_HIGH_ENA_LOW = 0x12,
                              RST_LOW_NO_ENA  = 0x20, RST_LOW_ENA_HIGH  = 0x21, RST_LOW_ENA_LOW  = 0x22
                             } flexinterface_t;
/*
#define SC_FLEXINT_MODULE(user_module_name)                                                 \
    template <bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>    \
    class user_module_name : public SyscFlexInt<CLKEDGE, FLEXINT>

#define SC_FLEXINT_MODULE_T(user_module_name,additional_template)                                               \
    template <additional_template, bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>   \
    class user_module_name : public SyscFlexInt<CLKEDGE, FLEXINT>
*/

template <bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class SyscFlexInt : public ::sc_core::sc_module {
public:
    typedef  sc_lv<1>       rst_t;
    typedef  sc_lv<1>       ena_t;

    // ---------------------      Ports      ---------------------
    sc_in_clk            clock{"clock"};    // Clock input
    sc_in<rst_t>         reset{"reset"};    // Asynchronous reset input
    sc_in<ena_t>         enable{"enable"};  // Synchronous enable input

    // Constructor
    typedef SyscFlexInt<CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;
    SyscFlexInt(::sc_core::sc_module_name name) : ::sc_core::sc_module(name) {
        flexintDecode((uint8_t)((FLEXINT & 0x00F0) >> 4), &use_rst, &rst_level);
        flexintDecode((uint8_t)(FLEXINT & 0x000F), &use_ena, &ena_level);
    }

    void doSensitive() {
        if (CLKEDGE) {
            sensitive << clock.pos();
        } else {
            sensitive << clock.neg();
        }
        if (use_rst) {
            sensitive << reset;
        }
    }

    bool bResetAction() { return use_rst && (reset.read() == rst_level); }
    bool bEnableAction() { return !use_ena || (enable.read() == ena_level); }

    void before_end_of_elaboration() {
        if (!use_rst && (reset.bind_count() == 0)) {
            reset.bind(rst_defbind);
        }
        if (!use_ena && (enable.bind_count() == 0)) {
            enable.bind(ena_defbind);
        }
    }

    void flexintDecode(const uint8_t f, bool *use, bool *level) {
        switch (f) {
        case 0x01:
            *use = true; *level = true;
            break;
        case 0x02:
            *use = true; *level = false;
            break;
        default:
            *use = false; *level = false;
        }
    }

    // --------------------- Local variables ---------------------
    sc_signal<rst_t>    rst_defbind{"IGNORErst"};         // To bind unused ports
    sc_signal<ena_t>    ena_defbind{"IGNOREena"};         // To bind unused ports
    bool                use_rst;
    bool                rst_level;
    bool                use_ena;
    bool                ena_level;
};

#include <type_traits>

template<typename>
  struct is_boolean
  : public std::false_type { };

template<>
  struct is_boolean<bool>
  : public std::true_type { };

#endif // SC_FLEXINT_H
