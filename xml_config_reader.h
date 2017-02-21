#pragma once

#include <boost\enable_shared_from_this.hpp>

#include "config.h"


namespace config {



struct config_reader :config_t {
    config_t& operator&(aggregation& t) {
        return operator>>(t);
    }
    config_t& operator&(config_name& t) {
        return operator>>(t);
    }
    //config_t& operator&(archive_safe_reference& t) {
    config_t& operator&(config_ptr& t) {
        return operator>>(t);
    }
    config_t& operator&(nvp& t) {
        return operator>>(t);
    }

    virtual config_t& operator>>(aggregation& t) = 0;
    virtual config_t& operator>>(config_name& t) = 0;
    //virtual config_t& operator>>(archive_safe_reference& t) = 0;
    virtual config_t& operator>>(config_ptr& t) = 0;
    virtual config_t& operator>>(nvp& t) = 0;
};

struct out_archive :config_t {
    config_t& operator&(nvp& t) {
        return operator<<(t);
    }
    virtual config_t& operator<<(const nvp& t) = 0;
};

//-------------------------------------------------------------------------------------------------
class xml_config_reader 
    :public config_reader
    ,public boost::enable_shared_from_this<xml_config_reader> {

    typedef xml_config_reader this_type;
public:
    typedef boost::shared_ptr<this_type> ptr;
public:
    xml_config_reader(const std::wstring& file_name) {
        reader_ptr xml_reader = config_factory::get_reader_instance(file_name);
        element = xml_reader->root();
    }

    xml_config_reader(element_ptr& element) 
        : element(element) {
    }

    static ptr load_from_file(const std::wstring& file_name) {
        return release_factory<this_type>::create_instance(file_name);
    }

    xml_config_reader& operator>>(aggregation& t) {
        bool get_all_childs = (t.name == L"*");
        for(element_ptr child = element->first_child(); !child->is_null(); child = child->next_sibling()) {
            if(get_all_childs || child->name() == t.name) {
                t.value.serialize(create_instance(child));
            }
        }
        return *this;
    }

    xml_config_reader& operator>>(config_name& name_store) {
        name_store.value = element->name();
        return *this;
    }

    //xml_config_reader& operator>>(config::archive_safe_reference& ref)
    xml_config_reader& operator>>(config_ptr& ref) {
        //ref.value = element->get_safe_reference();
        ref = shared_from_this();
        return *this;
    }

    xml_config_reader& operator>>(nvp& t) {
        t.set_value(element->attribute(t.name));
        return *this;
    }
private:
    static ptr create_instance(element_ptr& element) {
        return release_factory<this_type>::create_instance(element);
    }
private:
    element_ptr element;
};
//-------------------------------------------------------------------------------------------------



}
