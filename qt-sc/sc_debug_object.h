#ifndef SC_DEBUG_OBJECT_H
#define SC_DEBUG_OBJECT_H

#include <string>
#include <systemc.h>


class debug_object {

public:
    debug_object(sc_object *o);
    std::string report_value();
    bool isModule() { return (sc_kind == OBJ_KIND_MODULE); }
    bool isAdaptor() { return (sc_kind == OBJ_KIND_ADAPTOR); }
    bool isPort() { return (sc_kind == OBJ_KIND_PORT); }
    bool isSignal() { return (sc_kind == OBJ_KIND_SIGNAL); }
    bool isHierarchy() { return (sc_kind != OBJ_KIND_ADAPTOR) && (sc_kind != OBJ_KIND_OTHER); }
    bool isTraceable() { return (sc_kind == OBJ_KIND_CLOCK) || (sc_kind == OBJ_KIND_SIGNAL) || (sc_kind == OBJ_KIND_PORT); }
    std::string name() { if (nullptr != obj) return obj->name(); else return ""; }

private:
    typedef enum obj_kind_e {OBJ_KIND_ADAPTOR = 0,
                         OBJ_KIND_MODULE,
                         OBJ_KIND_CLOCK,
                         OBJ_KIND_PORT,
                         OBJ_KIND_SIGNAL,
                         OBJ_KIND_OTHER} obj_kind_t;
    sc_object *obj;
    obj_kind_t sc_kind;
};

#endif // SC_DEBUG_OBJECT_H
