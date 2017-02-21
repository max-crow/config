#ifndef __CONFIG_REFLECTION_ATTRIBUTES_H__
#define __CONFIG_REFLECTION_ATTRIBUTES_H__

#include <boost\format.hpp>
#include <boost\optional.hpp>
#include <list>


namespace config {
//namespace reflection{
;


struct attribute;
typedef boost::shared_ptr<attribute> attribute_ptr;

struct attribute {
	//enum attribute_process_result_flags {
	//	process_error = 0x80000000,
	//	process_ok = 0x01,
	//	process_changed = 0x02
	//};
	virtual std::wstring get_id() const = 0;
	virtual std::wstring get_name() const = 0;
	virtual std::wstring get_value() const = 0;
	virtual std::wstring to_string() const = 0;
	virtual boost::optional<std::wstring> find_error(boost::optional<std::wstring&> str_value) const = 0;
	//virtual attribute_ptr copy() const = 0;

	virtual ~attribute() {}
};

//template <class T>
class attribute_base 
	:public attribute {
public:
	attribute_base(std::wstring name, std::wstring id) 
		:name(name)
		,id(id) {
	}

	virtual std::wstring get_id() const {
		return id;
	}
	virtual std::wstring get_name() const {
		return name;
	}
	virtual std::wstring to_string() const {
		return boost::str(boost::wformat(L"%s: %s") % get_name() % get_value());
	}
protected:
	std::wstring name;
	std::wstring id;
};

//template <class T>
//class attribute_base : attribute {
//public:
//	virtual std::wstring get_name() const {
//		return name;
//	}
//	//virtual std::wstring get_value() const = 0;
//	//virtual std::wstring to_string() const = 0;
//	//virtual boost::optional<std::wstring> find_error(boost::optional<std::wstring&> str_value) const = 0;
//	//virtual attribute_ptr copy() const {
//	//	return new T(*this)
//	//}
//
//	//virtual ~attribute() {}	
//protected:
//	std::wstring name;
//};

//template <class T, class S>
//class nvp_attribute : attribute {
//public:
//	virtual std::wstring get_name() const {
//		return name;
//	}
//	virtual std::wstring get_value() const = 0;
//	//virtual std::wstring to_string() const = 0;
//	//virtual boost::optional<std::wstring> find_error(boost::optional<std::wstring&> str_value) const = 0;
//	//virtual attribute_ptr copy() const {
//	//	return new T(*this)
//	//}
//
//	//virtual ~attribute() {}	
//protected:
//	std::wstring name;
//};

//class optional : attribute_base {
//public:
//	optional(bool is_optional = true) 
//		:attribute_base(L"Optional")
//		,is_optional(is_optional) {
//	}
//	virtual std::wstring get_value() const = 0;
//	virtual std::wstring to_string() const = 0;
//	virtual boost::optional<std::wstring> find_error(boost::optional<std::wstring&> str_value) const = 0;
//	virtual attribute_ptr copy() const = 0;
//
//	virtual ~attribute() {}	
//private:
//	bool is_optional;
//};

class attributes_t {
	typedef std::list<attribute_ptr> collection;
public:
	typedef collection::iterator iterator;
	typedef collection::const_iterator const_iterator;

	void add(attribute_ptr new_value) {
		items.push_back(new_value);
	}

	iterator begin() {
		return items.begin();
	}
	const_iterator begin() const {
		return items.begin();
	}
	iterator end() {
		return items.end();
	}
	const_iterator end() const {
		return items.end();
	}
protected:
	collection items;
};


struct attributable {
	virtual const attributes_t& attributes() = 0;
	virtual ~attributable() {}
};


template <class I>
class attributable_impl 
	:public I {
public:
    template<class T>
    attributable_impl(T& param1) 
        :I(param1) {
    }

	const attributes_t& attributes() {
		return attrs;
	}

	template <class A>
	attributable_impl& operator %(A& attr) {
		boost::shared_ptr<A> acopy(new A(attr));
		attrs.add(acopy);
		return *this;
	}
protected:
	attributes_t attrs;
};


//}
}

#include "attributes\attribute_type.h"
#include "attributes\attribute_optional.h"


#endif