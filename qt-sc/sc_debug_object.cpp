#include <sstream>
#include "sc_debug_object.h"

#define CLASSIFY_INFO(sztype) \
    cout << "DEBUG :: object " << this->obj->name() << "(" << this->obj->kind() << ") classified as " << sztype << endl

#define CLASSIFY_OBJECT(typ,sck,sztype) \
    if ( nullptr != dynamic_cast<typ *>(o) ) { \
        this->sc_kind = sck; \
        /* CLASSIFY_INFO(sztype); */ \
        return; \
    }

debug_object::debug_object(sc_object *o)
    : obj(o)
    , sc_kind(OBJ_KIND_OTHER)
{
    if (!strcmp(o->kind(), "sc_trace_adaptor")) {
        this->sc_kind = OBJ_KIND_ADAPTOR;
        //CLASSIFY_INFO("trace adaptor");
        return;
    }
    CLASSIFY_OBJECT(sc_module,OBJ_KIND_MODULE,"module");
    CLASSIFY_OBJECT(sc_clock,OBJ_KIND_CLOCK,"clock");
    CLASSIFY_OBJECT(sc_prim_channel,OBJ_KIND_SIGNAL,"signal");
    CLASSIFY_OBJECT(sc_port_base,OBJ_KIND_PORT,"port (any)");
    // CLASSIFY_INFO("unknown");
}


#define REPORT_VALUE_AS(typ) \
    if (nullptr != dynamic_cast<typ *>(obj)) { \
        ss << (dynamic_cast<typ *>(obj))->read(); \
        return ss.str(); \
    }

#define REPORTALL_VALUE_AS(typ) \
    REPORT_VALUE_AS(sc_signal<typ>) \
    REPORT_VALUE_AS(sc_in<typ>) \
    REPORT_VALUE_AS(sc_out<typ>) \
    REPORT_VALUE_AS(sc_inout<typ>)

std::string debug_object::report_value() {
    std::stringstream ss;

    ss << obj->name() << "=";

    REPORTALL_VALUE_AS(bool);
    REPORTALL_VALUE_AS(char);
    REPORTALL_VALUE_AS(unsigned char);
    REPORTALL_VALUE_AS(signed char);
#ifdef _GLIBCXX_USE_WCHAR_T
    REPORTALL_VALUE_AS(wchar_t);
#endif
    REPORTALL_VALUE_AS(char16_t);
    REPORTALL_VALUE_AS(char32_t);
    REPORTALL_VALUE_AS(unsigned short);
    REPORTALL_VALUE_AS(unsigned int);
    REPORTALL_VALUE_AS(unsigned long);
    REPORTALL_VALUE_AS(unsigned long long);
    REPORTALL_VALUE_AS(short);
    REPORTALL_VALUE_AS(int);
    REPORTALL_VALUE_AS(long);
    REPORTALL_VALUE_AS(long long);
    REPORTALL_VALUE_AS(uint8_t);
    REPORTALL_VALUE_AS(uint16_t);
    REPORTALL_VALUE_AS(uint32_t);
    REPORTALL_VALUE_AS(uint64_t);
    REPORTALL_VALUE_AS(int8_t);
    REPORTALL_VALUE_AS(int16_t);
    REPORTALL_VALUE_AS(int32_t);
    REPORTALL_VALUE_AS(int64_t);
    REPORTALL_VALUE_AS(float);
    REPORTALL_VALUE_AS(double);
    REPORTALL_VALUE_AS(sc_lv<1>);
    REPORTALL_VALUE_AS(sc_lv<2>);
    REPORTALL_VALUE_AS(sc_lv<3>);
    REPORTALL_VALUE_AS(sc_lv<4>);
    REPORTALL_VALUE_AS(sc_lv<8>);
    REPORTALL_VALUE_AS(sc_lv<16>);
    REPORTALL_VALUE_AS(sc_lv<24>);
    REPORTALL_VALUE_AS(sc_lv<32>);
    REPORTALL_VALUE_AS(sc_lv<64>);

    return std::string(ss.str());
}
