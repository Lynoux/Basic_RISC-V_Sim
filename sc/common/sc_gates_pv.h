#ifndef SC_GATES_PV_H
#define SC_GATES_PV_H

#include <QtXml>
//#include <QDomNodeList>
#include <systemc.h>

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscLogicGate_pv : public ::sc_core::sc_module {
public:
    static_assert(N >= 2, "SyscLogicGate_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscLogicGate_pv DESIGN ERROR: W must be >= 1");

    typedef  sc_lv<W>        data_t;
    sc_vector<sc_in<data_t>> d;         // Data input(s)
                                        // sc_vector of ports; name can be initialized in the constructor to 'd_0', 'd_1', and so on (instead of 'port_0', 'port_1', etc.)
                                        // see https://stackoverflow.com/questions/35425052/how-to-initialize-a-systemc-port-name-which-is-an-array/35535730#35535730
    sc_out<data_t>          y{"y"};     // Data output

    typedef SyscLogicGate_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscLogicGate_pv<N,W>(::sc_core::sc_module_name name) : ::sc_core::sc_module(name), d("d", N) {
        SC_METHOD(combinational);
        for (unsigned int i=0; i < N; i++) {
            sensitive << d[i];
        }
    }

    virtual void combinational () {
        data_t result(SC_LOGIC_X);
        y.write(result);
    }

    static bool find(QString id, QDomNodeList children) {
        for (int i = 0; i < children.size(); i++) {
            QDomElement child = children.at(i).toElement();
            if (child.attribute("id").toUpper() == id) {
                return true;
            }
        }
    }

    static int getNInputs(QString gate, bool &ok) {
        int res = -1;
        ok = false;
        (void)gate;
        // Some code here to extract n from gate:
        // Gates with single-bit ports -> and (= and2), and2, andn, nand, ... both lowercase and uppercase
        //        number of inputs (N) ->  2              2     n     2
        //          bits per input (W) ->  1              1     1     1
        // Gates with w-bit ports -> and_w (= and2_w), and2_w, andn_w, nand_w, ... both lowercase and uppercase
        //   number of inputs (N) ->  2                   2      n       2
        //     bits per input (W) ->  w                   w      w       w
        // ...
        // In the end:
        // a) any non-numerical string ended by a number: XXXXXXXnnnn
        // b) any non-numerical string ended by a two numbers separated by _: XXXXXXXnnnn_wwww
        // Resort to Qt Regular Expression x for help about extracting this
        return res;
    }

    static int getWInputs(QString gate, bool &ok) {
        int res = -1;
        ok = false;
        (void)gate;
        // Some code here to extract w from gate:
        // Gates with single-bit ports -> and (= and2), and2, andn, nand, ... both lowercase and uppercase
        //        number of inputs (N) ->  2              2     n     2
        //          bits per input (W) ->  1              1     1     1
        // Gates with w-bit ports -> and_w (= and2_w), and2_w, andn_w, nand_w, ... both lowercase and uppercase
        //   number of inputs (N) ->  2                   2      n       2
        //     bits per input (W) ->  w                   w      w       w
        // ...
        // In the end:
        // a) any non-numerical string ended by a number: XXXXXXXnnnn
        // b) any non-numerical string ended by a two numbers separated by _: XXXXXXXnnnn_wwww
        // Resort to Qt Regular Expression x for help about extracting this
        return res;
    }

    static bool validate(QString gate, QString id, QDomElement gElement) {
        QDomNodeList children = gElement.elementsByTagName("id");
        bool ninputs_ok;

        // To validate the logic gate is in the catalog, only the number of inputs is required
        // (the bits/input is irrelevant at this moment)
        int ninputs = getNInputs(id, ninputs_ok);

        // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        // Rework this part to accept: and, and_wide, and2, and2_wide, ...,
        // AND2, ANDn, AND2_WIDE, ANDn_WIDE, (lowercase, uppercase, camelcase)
        // Being n the number of inputs (D_0, D_1, ..., D_n-1)
        // Y is the output
        // @remark: Be very "picky" here, reject *anything that doesn't match perfectly
        if (!id.toUpper().startsWith(gate)) {
            return false;
        }
        if (!ninputs_ok) {
            return false;
        }
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        if (!find("y", children)) {
            return false;
        }
        for (auto i = 0; i < ninputs; i++) {
            // For the SyscLogicGate class and its derivatives,
            // it should be "d" below (not "d_")
            if (!find("d_" + QString::number(i), children)) {
                return false;
            }
        }

        return true;
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscAnd_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscAnd_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscAnd_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscAnd_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscAnd_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result &= operand;
        }
        BASE_MODULE::y.write(result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("AND", id, gElement);
    }

};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscNand_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscNand_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscNand_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscNand_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscNand_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result &= operand;
        }
        BASE_MODULE::y.write(~result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("NAND", id, gElement);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscOr_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscOr_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscOr_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscOr_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscOr_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result |= operand;
        }
        BASE_MODULE::y.write(result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("OR", id, gElement);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscNor_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscNor_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscNor_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscNor_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscNor_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result |= operand;
        }
        BASE_MODULE::y.write(~result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("NOR", id, gElement);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscXor_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscXor_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscXor_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscXor_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscXor_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result ^= operand;
        }
        BASE_MODULE::y.write(result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("XOR", id, gElement);
    }
};

template <unsigned int N = 2,
          unsigned int W = 1>
class SyscXnor_pv : public SyscLogicGate_pv<N,W> {
public:
    static_assert(N >= 2, "SyscXnor_pv DESIGN ERROR: N must be >= 2");
    static_assert(W >= 1, "SyscXnor_pv DESIGN ERROR: W must be >= 1");
    typedef SyscLogicGate_pv<N,W> BASE_MODULE;
    typedef SyscXnor_pv<N,W> SC_CURRENT_USER_MODULE;
    SyscXnor_pv<N,W>(::sc_core::sc_module_name name) : SyscLogicGate_pv<N,W>(name) {
    }

    virtual void combinational () {
        auto i = N-1;
        auto result = BASE_MODULE::d[0].read();
        for (; i > 0; i--) {
            auto operand = BASE_MODULE::d[i].read();
            result ^= operand;
        }
        BASE_MODULE::y.write(~result);
    }

    static bool validate(QString id, QDomElement gElement) {
        return SyscLogicGate_pv<>::validate("XNOR", id, gElement);
    }
};

#endif // SC_GATES_PV_H
