#pragma once

#include <boost/optional.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/type_traits.hpp>

#include "core/transport/transport.h"

namespace config {

 
struct aggregation;
struct config_t;
struct config_name;
struct nvp;

typedef boost::shared_ptr<config_t> config_ptr;

struct config_t 
    :releasable {
    virtual config_t& operator&(aggregation& t) = 0;
    virtual config_t& operator&(config_name& t) = 0;
    virtual config_t& operator&(config_ptr& t) = 0;
    virtual config_t& operator&(nvp& t) = 0;
};

struct reader;
//struct node;
//struct node_list;
//struct named_node_map;
//struct safe_element_reference;
struct element;
//struct element_list;
struct config_t;

typedef boost::shared_ptr<reader> reader_ptr;
//typedef boost::shared_ptr<node> node_ptr;
//typedef boost::shared_ptr<node_list> node_list_ptr;
//typedef boost::shared_ptr<named_node_map> named_node_map_ptr;
//typedef boost::shared_ptr<safe_element_reference> safe_element_reference_ptr;
typedef boost::shared_ptr<element> element_ptr;
//typedef boost::shared_ptr<element_list> element_list_ptr;
typedef boost::shared_ptr<config_t> config_ptr;

struct reader : releasable {
    //virtual element get_document_element() = 0;
    virtual element_ptr root() = 0;
//    virtual element_list_ptr get_elements(const std::wstring& name = L"*") const = 0;
//    virtual std::wstring get_attribute(const std::wstring& name) const = 0;
};



//struct node_list : releasable {
//    virtual node_ptr item(size_t index) const = 0;
//    virtual size_t length() const = 0;
//};


//class node : releasable {
//    virtual std::wstring name() const = 0;
//    virtual std::wstring value() const = 0;
//    virtual bool is_null() const = 0;
//    virtual node_ptr first_child() const = 0;
//    virtual node_ptr last_child() const = 0;
//    virtual node_ptr previous_sibling() const = 0;
//    virtual node_ptr next_sibling() const = 0;
//    //virtual named_node_map_ptr attributes() const = 0;
//    //virtual size_t attribute_count() const = 0;
//    //virtual node_ptr attribute_node(size_t index) const = 0;
//    //virtual node_ptr attribute_node(const std::wstring& name) const = 0;
//};

//class attribute : node {
//};

//template <class T, class serializator>
//class aggregated_collection_item :public serializable {
//    typedef T value_type;
//    //typedef S serializator;
//public:
//    aggregated_array_item(T& value) 
//        :value(value)
//        ,first(first) {
//    }
//
//    void serialize(archive& a) {
//        for(iterator i=first; i!=last; i++) {
//            a & make_nvp(attr_name, *i);
//        }
//    }
//private:
//    T& value;
//};

//template <class T, class S>
//class aggregation_t :public aggregation {
//    
//    typedef T value_type;
//    typedef S serializator;
//
//    aggregation_t(const std::wstring& name, T& value) 
//        :nvp(name)
//        ,_value(value) {
//    }
//
//    serializable& value() {
//        return _value;
//    }
//private:
//    //std::wstring _name;
//    //S serializator;
//    T& _value;
//};

struct safe_element_reference : releasable {
    virtual element_ptr get_element() const = 0;
};


struct element : releasable {
    virtual std::wstring name() const = 0;
    virtual std::wstring value() const = 0;
    virtual bool is_null() const = 0;
    virtual boost::optional<std::wstring> attribute(const std::wstring& name) const = 0;
    //virtual element_list_ptr childs(const std::wstring& name = L"*") const = 0;
    //virtual short type() const = 0;
    virtual element_ptr first_child() const = 0;
    virtual element_ptr last_child() const = 0;
    virtual element_ptr previous_sibling() const = 0;
    virtual element_ptr next_sibling() const = 0;
    //virtual safe_element_reference_ptr get_safe_reference() const = 0;
};

struct element_list : releasable {
    virtual element_ptr item(size_t index) const = 0;
    virtual size_t length() const = 0;
};


//struct named_node_map : releasable {
//    virtual node_ptr item(size_t index) const = 0;
//    virtual node_ptr named_item(const std::wstring& name) const = 0;
//    virtual size_t length() const = 0;
//};

//-----------------------------------------------------------------------------


typedef reader_ptr create_reader_instance_f(const std::wstring& file_name);

struct config_factory {
    static reader_ptr get_reader_instance(const std::wstring& file_name) {
        HMODULE hmod = LoadLibrary(L"config.dll"); 
        GetLastError();
        create_reader_instance_f* create_reader_instance = (create_reader_instance_f*)GetProcAddress(hmod, "create_reader_instance");
        return create_reader_instance(file_name);
    }
};


}

#include "serialization/serialization.h"
