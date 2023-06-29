#ifndef SC_TERMINALS_H
#define SC_TERMINALS_H

#include <systemc.h>


template <unsigned int W = 1>
class SyscTerminal : public ::sc_core::sc_module {
public:
    static_assert(W >= 1, "SyscTerminal DESIGN ERROR: W must be >= 1");

    typedef  sc_lv<W>       data_t;

    typedef SyscTerminal<W> SC_CURRENT_USER_MODULE;
    SyscTerminal(::sc_core::sc_module_name name) : ::sc_core::sc_module(name)
      //, terminal_in_(nullptr), terminal_out_(nullptr)
    {
    }

    /*
    ~SyscTerminal()
    {
        if (terminal_in_ != nullptr) {
            delete terminal_in_;
        }
        if (terminal_out_ != nullptr) {
            delete terminal_out_;
        }
    }

    sc_in<data_t> *terminal_in() { return terminal_in_; };
    sc_out<data_t> *terminal_out() { return terminal_out_; };
    */

protected:
    //sc_in<data_t> *terminal_in_;
    //sc_out<data_t> *terminal_out_;

};

template <unsigned int W = 1>
class SyscIn : public SyscTerminal<W> {
public:
    static_assert(W >= 1, "SyscIn DESIGN ERROR: W must be >=1");

    unsigned int m_high_usteps, m_low_usteps, m_init_usteps;
    sc_logic m_init_value;

    typedef SyscTerminal<W> BASE_MODULE;
    typedef SyscIn<W> SC_CURRENT_USER_MODULE;

    sc_out<typename BASE_MODULE::data_t> terminal{"terminal"};

    SyscIn(::sc_core::sc_module_name name) : SyscTerminal<W>(name),
        m_high_usteps(2), m_low_usteps(2), m_init_usteps(2), m_init_value(SC_LOGIC_0)
      //, terminal("terminal")
    {
        //BASE_MODULE::terminal_out_ = new sc_out<typename BASE_MODULE::data_t>("terminal");
        SC_THREAD(Thread);
    }

    SyscIn(unsigned int high_usteps, unsigned int low_usteps, unsigned int init_usteps,
           sc_logic init_value, ::sc_core::sc_module_name name) : SyscTerminal<W>(name),
        m_high_usteps(high_usteps), m_low_usteps(low_usteps), m_init_usteps(init_usteps) , m_init_value(init_value)
      //, terminal("terminal")
    {
        //BASE_MODULE::terminal_out_ = new sc_out<typename BASE_MODULE::data_t>("terminal");
        SC_THREAD(Thread);
    }

    // @TODO: Insert a list of pairs (value, time) before the simulation is started
    // to generate an arbitray waveform (W-bits wide)

    void Thread () {
        if (m_init_usteps > 0) {
            //BASE_MODULE::terminal_out()->write(typename BASE_MODULE::data_t(m_init_value));
            terminal.write(typename BASE_MODULE::data_t(m_init_value));
            wait(m_init_usteps);
            m_init_value = (m_init_value == SC_LOGIC_0? SC_LOGIC_1 : SC_LOGIC_0);
        }
        while (1) {
            //BASE_MODULE::terminal_out()->write(typename BASE_MODULE::data_t(m_init_value));
            terminal.write(typename BASE_MODULE::data_t(m_init_value));
            wait(m_init_value == SC_LOGIC_0? m_low_usteps : m_high_usteps);
            m_init_value = (m_init_value == SC_LOGIC_0? SC_LOGIC_1 : SC_LOGIC_0);
        }
    }
};

template <unsigned int W = 1>
class SyscOut : public SyscTerminal<W> {
public:
    static_assert(W >= 1, "SyscOut DESIGN ERROR: W must be >=1");

    typedef  sc_lv<W>       data_t;

    typedef SyscTerminal<W> BASE_MODULE;
    typedef SyscOut<W> SC_CURRENT_USER_MODULE;

    sc_in<typename BASE_MODULE::data_t> terminal{"terminal"};

    SyscOut(::sc_core::sc_module_name name) : SyscTerminal<W>(name), terminal("terminal")
    {
        //BASE_MODULE::terminal_in_ = new sc_in<typename BASE_MODULE::data_t>("terminal");
    }

    // @TODO: What's the purpose of an output terminal?
};

#endif // SC_TERMINALS_H
