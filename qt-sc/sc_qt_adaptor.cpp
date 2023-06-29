#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <systemc.h>
#include "sc_qt_adaptor.h"
#include "sc_config.h"
#include "sc_run.h"
#include "sc_debug_object.h"


void scQtAdaptor::trace_module (sc_module *module) {
    auto children = module->get_child_objects();
    sc_object * o;

    for (size_t nchildren=0; nchildren < children.size(); nchildren++) {
        o = children.at(nchildren);
        debug_object debobj(o);
        register_object(debobj);
        if (debobj.isModule()) {
            trace_module(dynamic_cast<sc_module *>(o));
        }
    }
}

void scQtAdaptor::AdaptorThread () {
    sc_time ustep = sc_getMicrostep();
    sc_time now;

    while (sc_waitStateChange() == SC_ST_COMMAND_EXIT) {
        wait();
    }
    hierarchy();
    report(now);
    sc_NotifyUIFromSC();
    wait(1, SC_PS);
    while (1) {
        while (sc_waitStateChange() == SC_ST_COMMAND_EXIT) {
            wait();
        }
        wait(ustep-sc_time(1, SC_PS));
        now = sc_time_stamp();
        wait(1, SC_PS);
        report(now);
        sc_NotifyUIFromSC();
    }
}

void scQtAdaptor::end_of_elaboration() {
    sc_simcontext* simc = simcontext();
    std::vector<sc_object *> objects = sc_get_top_level_objects(simc);
    sc_object * o;

    if (simc != nullptr) {
        for (size_t i=0; i < objects.size(); i++){
            o = objects.at(i);
            debug_object debobj(o);
            register_object(debobj);
            if (debobj.isModule()) {
                trace_module(dynamic_cast<sc_module *>(o));
            }
        }
    }
}

void scQtAdaptor::register_object(debug_object &object)
{
    if (object.isHierarchy()) {
        m_hierarchy_objects.push_back(object);
    }
    if (object.isTraceable()) {
        m_traceable_objects.push_back(object);
    }
}

void scQtAdaptor::report(sc_time &now)
{
    std::vector<std::string> rep;

    //rep.push_back(sc_time_stamp().to_string());
    rep.push_back(now.to_string());
    //cout << "TRACE :: @" << sc_time_stamp() << endl;
    for (size_t i = 0; i < m_traceable_objects.size(); i++) {
        std::string value(m_traceable_objects.at(i).report_value());
        rep.push_back(value);
        //cout << "         " << value << endl;
    }
    sc_setReportFromSC(rep);
}

void scQtAdaptor::hierarchy()
{
    std::vector<std::string> hier;
    //cout << "HIERARCHY :: " << endl;
    for (size_t i = 0; i < m_hierarchy_objects.size(); i++) {
        std::string prefix;
        std::string node;
        std::string name(m_hierarchy_objects.at(i).name());
        std::string::difference_type n = std::count(name.begin(), name.end(), '.');
        prefix.append(3*n, '-');
        if (m_hierarchy_objects.at(i).isModule()) {
            node = " (node)";
        }
        hier.push_back(m_hierarchy_objects.at(i).name());
        //cout << "         " << prefix << name << node << endl;
    }
    sc_setHierarchyFromSC(hier);
}
