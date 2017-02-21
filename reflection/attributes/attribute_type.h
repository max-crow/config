#ifndef __CONFIG_REFLECTION_ATTRIBUTES_TYPE_H__
#define __CONFIG_REFLECTION_ATTRIBUTES_TYPE_H__

#include <boost\format.hpp>
#include <boost\type_traits.hpp>

namespace config {
//namespace reflection {
;

#define ATTRIBUTE_TYPE_ID "{5CDF2472-5DE1-4766-901D-B9F3D8AFA591}"

class attribute_type 
	:public attribute_base {
        //static const std::wstring id(""/*"{5CDF2472-5DE1-4766-901D-B9F3D8AFA591}"*/);
public:
    attribute_type(attribute_type& src)
        :attribute_base(L"Type", _L(ATTRIBUTE_TYPE_ID))
        ,value(src.value) {
    }
	template <class T>
	attribute_type(T& val) 
		:attribute_base(L"Type", _L(ATTRIBUTE_TYPE_ID)) {
		value = type_to_string(val);
	}
	template <class T>
	attribute_type(T& val, std::wstring type_name) 
		:attribute_base(L"Type", _L(ATTRIBUTE_TYPE_ID)) {
		value = type_name;
	}
	virtual std::wstring get_value() const {
		return value;
	}
	//virtual std::wstring to_string() const = 0;
	virtual boost::optional<std::wstring> find_error(boost::optional<std::wstring&> str_value) const {
		static boost::optional<std::wstring> false_res(false, L"");
		return false_res;
	}
private:
#define INT_TO_STRING(type)																	\
	std::wstring type_to_string(type&) {													\
		static std::wstring res(boost::str(boost::wformat(L"Int%d") % (sizeof(type)*8)));	\
		return res;																			\
	}

#define TYPE_TO_STRING(type, str)			\
	std::wstring type_to_string(type&) {	\
		static std::wstring res(L##str);	\
		return res;							\
	}

	INT_TO_STRING(short);
	INT_TO_STRING(int);
	INT_TO_STRING(long long);
	INT_TO_STRING(char);
	//TYPE_TO_STRING(char, "Char");
	TYPE_TO_STRING(bool, "Boolean");
	TYPE_TO_STRING(std::string, "String");
	TYPE_TO_STRING(std::wstring, "String");
//	TYPE_TO_STRING(, "");
//	TYPE_TO_STRING(, "");
	//std::wstring type_to_string(int&) {
	//	return boost::str(boost::wformat(L"Int%d") % (sizeof(int)*8));
	//}
	//std::wstring type_to_string(char&) {
	//	static std::wstring res(L"char");
	//	return res;
	//}
	//std::wstring type_to_string(std::string&) {
	//	static std::wstring res(L"string");
	//	return res;
	//}
private:
	std::wstring value;
};

//}
}

#endif
