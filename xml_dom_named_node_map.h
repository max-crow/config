//#pragma once
//
//#include <xercesc/dom/DOM.hpp>
//
//#include "config.h"
//
//XERCES_CPP_NAMESPACE_USE 
//
//namespace config { 
//namespace xml {
//namespace dom {
//
//class node;
//
//class named_node_map : public config::named_node_map
//{
//public:
//
//    named_node_map(DOMNamedNodeMap* map)
//        :map(map) {
//    }
//    named_node_map(const named_node_map& src)
//        :map(src.map) {
//    }
//
//    ~named_node_map() {
//    }
//
//
//    node item(size_t index) const {
//        return map->item(index);
//    }
//    node named_item(const std::wstring& name) const {
//        return map->getNamedItem(name.c_str());
//    }
//    size_t length() const {
//        return map->getLength();
//    }
//
//private:
//    DOMNamedNodeMap* map;
//};
//
//
//}
//}
//}
