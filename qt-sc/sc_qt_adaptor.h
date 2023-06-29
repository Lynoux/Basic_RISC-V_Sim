#ifndef SC_QT_ADAPTOR_H
#define SC_QT_ADAPTOR_H

#include <systemc.h>
#include "sc_debug_object.h"


SC_MODULE (scQtAdaptor) {

    SC_CTOR(scQtAdaptor) {
        SC_THREAD(AdaptorThread);
    }

    ~scQtAdaptor () {
    }

    void AdaptorThread ();

    virtual const char* kind() const { return "sc_trace_adaptor"; }
    void end_of_elaboration();
    void hierarchy();
    void report(sc_time &now);

private:

    void register_object(debug_object &obj);
    void trace_module (sc_module *module);
    std::vector<debug_object> m_hierarchy_objects;
    std::vector<debug_object> m_traceable_objects;
};

#endif // SC_QT_ADAPTOR_H
