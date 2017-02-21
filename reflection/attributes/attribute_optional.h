#ifndef __CONFIG_REFLECTION_ATTRIBUTES_OPTIONAL_H__
#define __CONFIG_REFLECTION_ATTRIBUTES_OPTIONAL_H__

//#include <boost\format.hpp>
//#include <boost\type_traits.hpp>

namespace config {
//namespace reflection {
;

#define ATTRIBUTE_OPTIONAL_ID "{298EE467-C36F-4447-A1B0-6F38DB8FCC0A}"

class attribute_optional 
	:public attribute_base {
    //static const std::wstring id(L"{298EE467-C36F-4447-A1B0-6F38DB8FCC0A}");
public:
	attribute_optional(bool val = true) 
		:attribute_base(L"Optional", _L(ATTRIBUTE_OPTIONAL_ID)) {
        static const std::wstring true_res(L"True");
        static const std::wstring false_res(L"False");
        value = (val)? true_res  : false_res; 
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
	std::wstring value;
};

//}
}

#endif
