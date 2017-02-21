#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <xercesc/dom/DOM.hpp>

#include "config.h"
//#include "xml_dom_node.h"
//#include "xml_dom_node_list.h"



XERCES_CPP_NAMESPACE_USE 

namespace config { 
//namespace xml {
//namespace dom {

class xd_element;

class xd_reader 
    :public reader
    ,public boost::enable_shared_from_this<xd_reader>
{
public:
    typedef boost::shared_ptr<xd_reader> ptr;
public:
    xd_reader(std::wstring file_uri);
    ~xd_reader(void);

    element_ptr root();

    xercesc_2_8::DOMDocument* get_source() const {
        return document;
    }
    //node_list get_tags(const std::wstring& name) const;
    //std::wstring get_attribute(const std::wstring& name) const;


private:
    //template <class T>
    //boost::shared_ptr<T> create_ptr(*T) {
    //    return boost::shared_ptr<T>(*T, releasator<T>());
    //}

private:
    //WARNING MaximV: parser ������ �������� �����!!! �� �����!!!
    //boost::shared_ptr<DOMBuilder> parser;
    DOMBuilder* parser;
    xercesc_2_8::DOMDocument *document;
};


//}
//}
}
