#pragma once

#include <boost/shared_ptr.hpp>

#include "xml_dom_reader.h"

namespace config { 

class factory
{
public:
    factory();
    ~factory();

    static void start() {
        assert(p_factory == NULL);
        if(p_factory != NULL) {
            throw std::exception("repeated factory::start()!");
        }
        p_factory = new factory();
    }
    static void stop() {
        assert(p_factory != NULL);
        if(p_factory == NULL) {
            throw std::exception("invalid factory::stop()!");
        }
        delete p_factory;
        p_factory = NULL;
    }

    static reader_ptr create_reader(const std::wstring& file_name) {
        assert(p_factory != NULL);
        if(p_factory == NULL) {
            throw std::exception("factory not started!");
        }
        reader_ptr res = release_factory<xd_reader>::create_instance(file_name);
        return res;
    }
private:
    static factory* p_factory;

//    transporter transporter;
};

}
