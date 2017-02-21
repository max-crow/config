#pragma once

#include <list>

#include "config.h"
#include "xml_dom_reader.h"

XERCES_CPP_NAMESPACE_USE 

namespace config { 

struct element_description {
    std::wstring name;
    int number;
};

class xd_safe_element_reference 
    :public safe_element_reference
{
    typedef std::list<element_description> xd_path;
    //xd_safe_element_reference(const xd_safe_element_reference& src) {}
public:

    xd_safe_element_reference(const xd_element& src) {
        DOMNode*  node = src.get_source();
        file_uri = node->getOwnerDocument()->getDocumentURI();
        for(; node!=NULL; node=node->getParentNode()) {
            element_description new_item;
            new_item.name = node->getNodeName();
            new_item.number = 0;
            for(DOMNode* sibling = node->getPreviousSibling(); sibling!=NULL; sibling = sibling->getPreviousSibling()) {
                if(sibling->getNodeType() == DOMNode::ELEMENT_NODE && new_item.name == sibling->getNodeName()) {
                    new_item.number++;
                }
            }
            path.push_front(new_item);
        }
    }

    ~xd_safe_element_reference() {
    }

    element_ptr get_element() const {
        xd_reader::ptr reader(release_factory<xd_reader>::create_instance(file_uri));
        DOMNode* node = reader->get_source()->getDocumentElement();
        for(xd_path::const_iterator itr=path.begin(); itr!=path.end(); itr++) {
            int i=0;
            for(node = node->getFirstChild(); node!=NULL; node->getNextSibling()) {
                if(node->getNodeType()==DOMNode::ELEMENT_NODE && itr->name == node->getNodeName()) {
                    if(itr->number <= i) {
                        assert(itr->number == i);
                        break;
                    } else {
                        i++;
                    }
                }
            }
            assert(node!=NULL); //����� �� ������ ���� ��� ���������. ������ ���������. ������� path
            if(node == NULL) {
                //WARNING MaximV: exception
                throw std::exception("Invalid path");
            }
        }
        return xd_element::create_instance(node, reader);
    }
private:
    std::wstring file_uri;
    xd_path path;
};

}