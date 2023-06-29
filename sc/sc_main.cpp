#include <pthread.h>
#include <systemc.h>
#include "sc_main.h"
#include "sc_run.h"
#include "sc_config.h"
#include "sc_qt_adaptor.h"
#include "sc_rst.h"
#include "sc_clk.h"
#include "sc_mux.h"
#include "sc_tri.h"
#include "sc_terminals.h"
#include "sc_gates.h"
#include "sc_gates_pv.h"
#include "sc_reg.h"
#include "sc_arith.h"



template <bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class U1block : public SyscFlexInt<CLKEDGE, FLEXINT> {

public:
    typedef  sc_lv<32>       data_t;
    typedef  sc_lv<1>        sel_t;
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<rst_t>      reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<ena_t>      enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<data_t> q{"q"};                  // Data output
    sc_in<data_t>  load{"load"};            // Load value (when mux selection = 1, d = load)
    sc_in<data_t>  incr{"incr"};            // Incr value (when mux selection = 0, d = q + incr)
    sc_in<sel_t>   sel{"sel"};              // Mux selection value

    typedef SyscFlexInt<CLKEDGE, FLEXINT> BASE_MODULE;
    typedef U1block<CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;

private:
    SyscMux<2,32>                 u3;
    SyscReg<32,CLKEDGE,FLEXINT>   u2;
    SyscAdd<2,32>                 a1;

    sc_signal<sc_lv<32>> wire1{"u3.d0,a1.y"};
    sc_signal<sc_lv<32>> wire2{"u3.y,u2.d"};

public:
    U1block(::sc_core::sc_module_name name)
        : SyscFlexInt<CLKEDGE,FLEXINT>(name)
        , u3("u3")
        , u2("u2")
        , a1("a1")
    {
        u3.d[0]->bind(wire1);
        u3.d[1]->bind(load);
        u3.sel(sel);
        u3.y(wire2);

        a1.d[0]->bind(u2.q);
        a1.d[1]->bind(incr);
        a1.y(wire1);

        u2.d.bind(wire2);
        u2.reset.bind(BASE_MODULE::reset);
        u2.clock.bind(BASE_MODULE::clock);
        u2.q.bind(q);
    }
};


/*
 * Create a separate thread to execute this code, the original systemc's main() code.
 * @code return sc_core::sc_elab_and_sim(argc, argv); @endcode
 */
void *scSimMain(void *args)
{
    (void)args;
    scQtAdaptor ADAPTOR("QT<->SC_bridge");
    sc_setRunResult(sc_core::sc_elab_and_sim(0, nullptr));
    return nullptr;
}


/*
 * Create design and run the simulation
 */
int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Inputs
    //SyscIn<1> clk(1, 1, 0, SC_LOGIC_0, "clk");
    sc_clock  clk("clk");
    //SyscIn<1> rst(4, 4, 0, SC_LOGIC_0, "rst");
    sc_signal<sc_lv<1>> rst("rst");

    //SyscIn<1> sel(2, 2, 0, SC_LOGIC_0, "sel");
    sc_signal<sc_lv<1>> sel("sel");
    //SyscIn<32> incr(4, 4, 0, SC_LOGIC_0, "incr");
    sc_signal<sc_lv<32>> incr("incr");
    //SyscIn<1> sel2(4, 4, 0, SC_LOGIC_0, "sel2");
    sc_signal<sc_lv<1>> load("load");
    //SyscIn<32> load(4, 4, 0, SC_LOGIC_0, "load");
    sc_signal<sc_lv<32>> val("val");

    // Outputs
    //SyscOut<32> y("y");
    sc_signal<sc_lv<32>> y("y");

    // Components
    SyscCnt<32, DEFAULT_CLKEDGE, RST_HIGH_NO_ENA> u4("u4");
    SyscMux_pv<2,32,1> u5("u5");
    U1block<true, RST_HIGH_NO_ENA> u1("u1");

    // Additional wires
    sc_signal<sc_lv<32>> wire1("u5.d0,u4.q");
    sc_signal<sc_lv<32>> wire2("u5.d1,u1.q");

    // Wiring/binding
    u4.reset(rst);
    u4.clock(clk);
    u4.q(wire1);

    u5.sel(sel);
    u5.d[0](wire1);
    u5.d[1](wire2);
    u5.y(y);

    u1.sel(load);
    u1.incr(incr);
    u1.load(val);
    u1.reset(rst);
    u1.clock(clk);
    u1.q(wire2);

    sc_start(); // Run forever

    return 0;
}
