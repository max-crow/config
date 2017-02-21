#pragma once

#include "core\utilities\string.hpp"

namespace config {


struct aggregation {
    aggregation(const std::wstring& name, serializable& value) 
        :name(name)
        ,value(value)  
        {
    }

    const std::wstring name;
    serializable& value;
};

 
inline aggregation make_aggregation(const std::wstring& name, serializable& value) {
    return aggregation(name, value);
}

#define CONFIG_SERIALIZATION_AGGR_(variable)                                                 \
    config::make_aggregation(_L(##BOOST_PP_STRINGIZE(variable)), variable)


//-----------------------------------------------------------------------------------------
class aggregated_config_ptr 
	:public serializable{
public:
    aggregated_config_ptr(config_ptr& cfg) 
		:cfg(cfg){
    }

	serializable& serialize(config_ptr a) {
        *a & cfg;
        return *this;
    }
protected:
    config_ptr& cfg;
};

class aggregation_for_config_ptr 
	:public aggregation {
public:
    aggregation_for_config_ptr(const std::wstring& name, config_ptr& cfg_ptr) 
		:cfg(cfg_ptr)
		,aggregation(name, cfg){
    }
protected:
    aggregated_config_ptr cfg;
};

inline aggregation_for_config_ptr make_aggregation(const std::wstring& name, config_ptr& value) {
    return aggregation_for_config_ptr(name, value);
}

//-----------------------------------------------------------------------------------------
template <class collection, class serializator>
class aggregated_item_value_collection :public serializable {
    typedef typename collection::iterator iterator;
    typedef typename iterator::value_type value_type;
public:
    aggregated_item_value_collection(collection& src) 
        :item_name(L"item")
        ,attr_name(L"value")
        ,src(src) {
    }

    void serialize(archive& a) {
    //virtual serializable& serialize(config_ptr cfg) { //dimonml
        for(iterator i=src.begin(); i!=src.end(); i++) {
            cfg & aggregation(item_name, nvp_t<T, serializator>(attr_name, *i));
            //if(boost::is_base_of<serializable, value_type>::value) {
            //    a & aggregation(L"item", *i);
            //} else {
            //    a & aggregation(L"item", nvp_t<value_type, serializator>(L"value", *i));
            //}            
        }
        //return *this; dimonml
    }
private:
    const std::wstring item_name;
    const std::wstring attr_name;
    collection& src;
};
//-----------------------------------------------------------------------------------------
template <class collection>
class collection_inserter{
public:
    typedef typename collection::iterator iterator;
    typedef typename collection::value_type value_type;

    static iterator insert(collection& c, value_type& val) {
        return c.insert(c.end(), val);
    }
    static value_type get_value(iterator& i) {
        return *i;
    }


};

template <class collection, class key_getter>
class map_inserter {
public:
    typedef typename collection::iterator iterator;
    typedef typename collection::mapped_type value_type;

    static iterator insert(collection& c, value_type& val) {
        return c.insert(collection::value_type(key_getter::get_key(val), val)).first;
    }
    static value_type get_value(iterator& i) {
        return i->second;
    }
};

template <class map_type>
class name_is_key {
public:
    //static typename map_type::key_type get_key(typename map_type::mapped_type& val) {
    template <class T>
    static typename map_type::key_type get_key(T& val) {
        return val.name;
    }
    template <>
    static typename map_type::key_type get_key(config_ptr& val) {
        std::wstring name;
        val->operator&(config::config_name(name));
        map_type::key_type res;
        ::utilities::convert_string(name, res);
        return res;
    }
};

template <class C>
inline name_is_key<C> make_name_is_key(C&) {
    return name_is_key<C>();
}

//-----------------------------------------------------------------------------------------
template <class collection, class item_serializator, class inserter>
class aggregated_class_collection :public serializable {
    typedef typename collection::iterator iterator;
    typedef typename iterator::value_type value_type;
public:
    aggregated_class_collection(collection& src, std::wstring item_name) 
        :item_name(item_name)
        ,src(src) {
    }

    serializable& serialize(config_ptr a) {
        *a & aggregation(item_name, item_aggregator(*this));
        //for(iterator i=src.begin(); i!=src.end(); i++) {
        //    a & aggregation(item_name, *i);
        //}
        return *this;
    }
private:
    const std::wstring item_name;
    collection& src;
    //collection::iterator in_iterator
private:
    class item_aggregator :public serializable { 
        aggregated_class_collection& src;
        iterator position;
    public:
        item_aggregator(aggregated_class_collection& src)
            :src(src)
            ,position(src.src.begin()) {
        }
        serializable& serialize(config_ptr a) {
            //WARNING MaximV: ���������� �� ��������? 
            //���� ��������� ������, ������ �� ����� ���������, 
            //����� - ��������� �����/������������ ����������
            //�����, ���-����, ������� �������� ������: ����� �� ��� ������?
            if(position == src.src.end()) {
                //collection::value_type val;
                //iterator i = src.src.insert(src.src.end(), collection::value_type()); //WARNING MaximV: ����� ��� ����� allocator?
                //i->serialize(a);//a & make_nvp(src.item_name.c_str(), *i);

                
                inserter::value_type val;
                serialize_value(val, a);
                //val.serialize(a);


                inserter::insert(src.src, val);
                position = src.src.end();
                return *this;
            } else {
                for(iterator i=src.src.begin(); i!=src.src.end(); i++) {
                    //inserter::get_value(i).serialize(a);
                    serialize_value(inserter::get_value(i), a);
                }
                return *this;
            }
        }

        static void serialize_value(serializable& val, config_ptr& a) {
            val.serialize(a);
        }
        static void serialize_value(config_ptr& val, config_ptr& a) {
            *a & val;
        }
    };
};
//-----------------------------------------------------------------------------------------
#define CONFIG_SERIALIZATION_AGGR_IV_COLLECTION_(name)                                  \
    config::aggregation(                                                 \
                    _L(##BOOST_PP_STRINGIZE(name)),                                     \
                    config::make_aggregated_item_value_collection(name))

template<class C>
inline aggregated_item_value_collection<C, default_serializator<typename C::value_type>> make_aggregated_item_value_collection(C& collection){
    return aggregated_item_value_collection<C, default_serializator<typename C::value_type> >(collection);
}

#define CONFIG_SERIALIZATION_AGGR_CLASS_COLLECTION_(name)                       \
    config::aggregation(                                         \
    /*config::make_aggregation(                                    */\
                    _L(##BOOST_PP_STRINGIZE(name)),                             \
                    config::make_aggregated_class_collection(    \
                        name,                                                   \
                        config::make_collection_inserter(name),  \
                        L"item"                                                 \
                    )                                                           \
    )

template<class C, class inserter>
inline aggregated_class_collection  <   C, 
                                        default_serializator<typename C::value_type>, 
                                        inserter
                                    > 
                                    make_aggregated_class_collection(   C& collection, 
                                                                        inserter,
                                                                        std::wstring item_name) {
    return aggregated_class_collection<C, default_serializator<typename C::value_type>, inserter>(collection, item_name);
}

template <class C>
inline collection_inserter<C> make_collection_inserter(C&) {
    return collection_inserter<C>();
}

#define CONFIG_SERIALIZATION_AGGR_NAMED_MAP_(name)                                          \
    config::aggregation(                                                     \
                    _L(##BOOST_PP_STRINGIZE(name)),                                         \
                    config::make_aggregated_class_collection(                \
                            name,                                                           \
                            config::make_map_inserter(                       \
                                name,                                                       \
                                config::make_name_is_key(name)               \
                            ),                                                              \
                            L"*"                                                            \
                    )                                                                       \
    ) 

template <class C, class key_extractor>
inline map_inserter<C, key_extractor> make_map_inserter(C&, key_extractor) {
    return map_inserter<C, key_extractor>();
}




}
