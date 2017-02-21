//#pragma once
//
//#include <xercesc/dom/DOM.hpp>
//
//#include "config.h"
//#include "xml_dom_node.h"
//
//XERCES_CPP_NAMESPACE_USE 
//
//namespace xml {
//namespace dom {
//
//class node_list : public config::node_list
//{
//public:
//
//    node_list(DOMNodeList* list) {
//    }
//
//    node_list(const node_list& src)
//        :list(src.list) {
//    }
//
//    ~node_list() {
//    }
//
//    node item(size_t index) const {
//        return node(list->item(index));
//    }
//
//    size_t length() const {
//        return list->getLength();
//    }
//private:
//    DOMNodeList* list;
//};
//
//
//}
//}