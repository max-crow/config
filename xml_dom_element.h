#pragma once

#include <boost/optional.hpp>
#include <xercesc/dom/DOM.hpp>

#include "config.h"
#include "xml_dom_reader.h"

XERCES_CPP_NAMESPACE_USE 

namespace config { 
//namespace xml {
//namespace dom {

//class xd_in_archive;
class xd_reader;
    
class xd_element 
    :public element
    //,public in_archive
{
public:

    xd_element(DOMElement* ptr, xd_reader::ptr reader)
        :ptr(ptr)
        ,reader(reader) {
    }
    xd_element(const xd_element& src)
        :ptr(src.ptr) 
        ,reader(src.reader) {
    }

    ~xd_element() {
    }

    //element_list_ptr childs(const std::wstring& name = L"*") const;

    std::wstring name() const {
        return ptr->getNodeName();
    }

    std::wstring value() const {
        return ptr->getNodeValue();
    }

    bool is_null() const {
        return (ptr == NULL);
    }

    element_ptr first_child() const {
short t = ptr->getNodeType();
        return find_element<true>(ptr->getFirstChild());
    }

    element_ptr last_child() const {
        return find_element<false>(ptr->getLastChild());
    }

    element_ptr previous_sibling() const {
        return find_element<false>(ptr->getPreviousSibling());
    }

    element_ptr next_sibling() const {
        return find_element<true>(ptr->getNextSibling());
    }

	boost::optional<std::wstring> attribute(const std::wstring& name) const {
		const XMLCh* attr = ptr->getAttribute(name.c_str());
		return boost::optional<std::wstring>((attr!=0), attr);
    }

    DOMElement* get_source() const {
        return ptr;
    }

//    safe_element_reference_ptr get_safe_reference() const;

    //short type() const {
    //    return ptr->getNodeType();
    //}
    //archive& get_iarchive() {
    //    return *this;
    //}

////---------------- in_archive ----------------
//    archive& operator>>(aggregation& t) {
//        element_list_ptr list = childs(t.name);
//        assert(list->length() == 1);
//        t.value.serialize(list->item(0)->get_iarchive());
//        return *this;
//    }
//    archive& operator>>(archive_name& name) {
//        name.value = this->name();
//        return *this;
//    }
//    archive& operator>>(nvp& t) {
//        t.set_value(attribute(t.name));
//        return *this;
//    }
////--------------------------------------------

    element_ptr create_instance(DOMNode* node) const {
        return create_instance(node, reader);
    }

    static element_ptr create_instance(DOMNode* node, xd_reader::ptr reader) {
        if(node != NULL) {
            short ntype = node->getNodeType();
            assert(ntype == DOMNode::ELEMENT_NODE);
            if(ntype != DOMNode::ELEMENT_NODE) {
                //WARNING MaximV: exception
                throw std::exception("Invalid node type");
            }
        }
        return release_factory<xd_element>::create_instance((DOMElement*)node, reader);
    }

private:
    template <bool move_next>
    element_ptr find_element(DOMNode* node) const {
        while(node != NULL) {
            if(node->getNodeType() == DOMNode::ELEMENT_NODE) {
                return create_instance(node);
            }
            node = (move_next)? node->getNextSibling() : node->getPreviousSibling();
        }
        return create_instance(NULL);
    }

private:
    xd_reader::ptr reader;
    DOMElement* ptr;
    //DOMNode* ptr;
};


//class xd_in_archive :public in_archive {
//public:
//    archive& operator>>(const nvp& t) {
//    }
//    archive& operator>>(const aggregation& t) = 0;
//private:
//    xd_element&
//};
    

//}
//}
}
