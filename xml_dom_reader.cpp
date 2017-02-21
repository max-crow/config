#include "xml_dom_reader.h"


#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "xml_dom_element.h"

using namespace config;//::xml::dom;
using namespace boost;
using namespace std;
XERCES_CPP_NAMESPACE_USE 


xd_reader::xd_reader(std::wstring file_uri) {
    try {
        XMLPlatformUtils::Initialize();


        DOMImplementationLS *impl = (DOMImplementationLS*)DOMImplementationRegistry::getDOMImplementation(L"");//tempStr);
        //parser = shared_ptr<DOMBuilder>(
        //    impl->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0),
        //    releasator<DOMBuilder>()
        //);
        parser = impl->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);


        // optionally you can set some features on this builder
        //if (parser->canSetFeature(XMLUni::fgDOMValidation, true))
        //    parser->setFeature(XMLUni::fgDOMValidation, true);
        //if (parser->canSetFeature(XMLUni::fgDOMNamespaces, true))
        //    parser->setFeature(XMLUni::fgDOMNamespaces, true);
        //if (parser->canSetFeature((XMLUni::fgDOMDatatypeNormalization, true))
        //    parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);


        // optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
        // and set it to the builder
        //scoped_ptr<MyDOMErrorHandler> errHandler(new myDOMErrorHandler());
        //parser->setErrorHandler(errHandler);

        //char* xmlFile = ;

        try {
            document = parser->parseURI(file_uri.c_str());
            if(document == NULL) {
                // WARNING MaximV: exception
                throw std::exception("file opening failed");
            }
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
                // WARNING MaximV: exception
            std::exception e(message);
            XMLString::release(&message);
            throw e;
            return;
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
                // WARNING MaximV: exception
            std::exception e(message);
            XMLString::release(&message);
            throw e;
            return;
        }
        catch (...) {
                // WARNING MaximV: exception
            std::exception e("Unhandled Exception");
            return;
        }

    }
    catch (const XMLException& toCatch) {
        // WARNING MaximV: XMLPlatformUtils::Initialize(); exception
        return ;
    }
}

xd_reader::~xd_reader(void) {
    parser->release();
    XMLPlatformUtils::Terminate();
}

element_ptr xd_reader::root() {
    DOMElement *p = document->getDocumentElement();
    element_ptr res = release_factory<xd_element>::create_instance(p, shared_from_this());
    return res;
}


//node_list reader::get_tags(const wstring& name) const {
//    return document->getElementsByTagName(name.c_str());
//    //XMLPlatformUtils::Terminate
//}
//
//wstring reader::get_attribute(const wstring& name) const {
//    DOMNamedNodeMap* attrs = document->getAttributes();
//    DOMNode* node = attrs->getNamedItem(name.c_str());
//    if(!node) {
//        // WARNING MaximV: xml_reader::get_attribute !!!!!!!!!!!!!
//        assert(node);
//        throw std::exception("invalide attribute name");
//    }
//    const XMLCh* value = node->getNodeValue();
//    return wstring(value);
//}
