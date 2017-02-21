#pragma once

#include "reflection\attributes.h"

namespace config {
;


struct nvp : attributable
{
    const std::wstring name;

    nvp(const std::wstring& name) 
        :name(name) {
    }

    virtual const std::wstring get_value() = 0;
    virtual void set_value(boost::optional<std::wstring>) = 0;
};

template <class T, class S>
class nvp_t 
	:public attributable_impl<nvp>
    ,public serializable
{
public:    
    typedef T value_type;
    typedef S serializator;

    nvp_t(const std::wstring& name, T& value) 
        :attributable_impl<nvp>(name)
        ,_value(value) {
    }

    serializable& serialize(config_ptr a) {
        *a & *this;
        return *this;
    }

    virtual const std::wstring get_value() {
        return serializator::to_string(_value);
    }                    
    virtual void set_value(boost::optional<std::wstring> str) {   
        serializator::parse(_value, str.get());
    }
private:
    T& _value;
};

//template <class T, class S>
//class attributed_nvp 
//    :public nvp 
//    //,public serializable
//{
//public:    
//    typedef T value_type;
//    typedef S serializator;
//
//    nvp_t(const std::wstring& name, T& value) 
//        :nvp(name)
//        ,_value(value) {
//    }
//
//    serializable& serialize(config_ptr a) {
//        *a & *this;
//        return *this;
//    }
//
//    virtual const std::wstring get_value() {
//        return serializator::to_string(_value);
//    }
//    virtual void set_value(const std::wstring& str) {   
//        serializator::parse(_value, str);
//    }
//private:
//    T& _value;
//};
//

#define CONFIG_SERIALIZATION_NVP_(name)                              \
    config::make_nvp(_L(##BOOST_PP_STRINGIZE(name)), name)

template<class T>
inline nvp_t<T, default_serializator<T> > make_nvp(const wchar_t* name, T& t){
    return nvp_t<T, default_serializator<T> >(name, t);
}



}
