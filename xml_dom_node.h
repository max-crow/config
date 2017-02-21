//#pragma once
//
//#include <xercesc/dom/DOM.hpp>
//
//#include "config.h"
//
//XERCES_CPP_NAMESPACE_USE 
//
//namespace config { 
//
//class named_node_map;
//    
//class xd_node : public config::node
//{
//public:
//
//    xd_node(DOMNode* xnode)
//        :xnode(xnode) {
//    }
//    xd_node(const xd_node& src) 
//        :xnode(src.xnode) {
//    }
//
//    ~xd_node() {
//    }
//
//    std::wstring name() const {
//        return xnode->getNodeName();
//    }
//
//    std::wstring value() const {
//        return xnode->getNodeValue();
//    }
//
//    bool is_null() const {
//        return (xnode == NULL);
//    }
//
//    node_ptr first_child() const {
//        return xnode->getFirstChild();
//    }
//
//    node_ptr last_child() const {
//        return xnode->getLastChild();
//    }
//
//    node_ptr previous_sibling() const {
//        return xnode->getPreviousSibling();
//    }
//
//    node_ptr next_sibling() const {
//        return xnode->getNextSibling();
//    }
//
//    named_node_map_ptr attributes() const;
//private:
//    DOMNode* xnode;
//};
//
//
//}
//
//#include "xml_dom_named_node_map.h"
//
//inline
//config::named_node_map_ptr config::xml::dom::node::attributes() const {
//    return xnode->getAttributes();
//}
