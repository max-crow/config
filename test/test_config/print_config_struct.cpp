#include "stdafx.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <map>

#include "core\config\config.h"


namespace {

// "атрибут" в xml`е
struct tree_nvp {
    std::wstring name;
    std::list<std::wstring> attributes; // тип, optional/не-optional...

    tree_nvp(std::wstring name)
        :name(name) {
    }
};

// "тег" в xml`е
struct tree 
    :public config::config_t 
{
    typedef boost::shared_ptr<tree> ptr;

    std::wstring name;
    std::list<tree_nvp> nvps;
    std::list<ptr> childs;


    tree(std::wstring name) 
        :name(name) {
    }
    virtual config::config_t& operator&(config::aggregation& t) { // вызывается, если у текущего xml-тега есть child`ы
        ptr child = release_factory<tree>::create_instance(t.name);
        t.value.serialize(child);
        childs.push_back(child);
        return *this;
    }
    virtual config::config_t& operator&(config::config_name& t) { // имя текущего тега
        name += L"*";
        return *this;
    }
    virtual config::config_t& operator&(config::config_ptr& t) { // "void*" на текущий тег
        name += L"&";
        return *this;
    }
    virtual config::config_t& operator&(config::nvp& t) { // вызывается, если у текущего xml-тега есть xml-атрибуты (name-value-pair)
        tree_nvp nvp(t.name);
        BOOST_FOREACH(config::attribute_ptr attr, t.attributes()) {
            nvp.attributes.push_back(attr->to_string());
        }
        nvps.push_back(nvp);
        return *this;
    }
};

void print(const tree_nvp& t) {
    std::wcout << L" " << t.name;
    if(t.attributes.size() > 0) {
        std::wcout << L"("; 
        BOOST_FOREACH(const std::wstring& attr, t.attributes) {
            std::wcout << attr << L", ";
        }
        std::wcout << L"\b\b)"; 
   }
}

void print(tree::ptr t, int level) {
    std::wstring spaces(level*4, L' ');
    std::wcout << spaces << L"<" << t->name;
    BOOST_FOREACH(const tree_nvp& nvp, t->nvps) {
        print(nvp);
    }
    if(t->childs.size() < 1) {
        std::wcout << L" />" << std::endl;
    } else {
        std::wcout << L">" << std::endl;
        BOOST_FOREACH(const tree::ptr& child, t->childs) {
            print(child, level+1);
        }
        std::wcout << spaces << L"</" << t->name << L">" << std::endl;
    }
}



struct period_cfg :public config::serializable {
    int minutes;

	BEGIN_CONFIG_MAP()
        CONFIG_NVP(minutes);
	END_CONFIG_MAP()
}; 

struct db_cfg :public config::serializable {  
    std::string connection_string;
    std::string query;

	BEGIN_CONFIG_MAP()
        CONFIG_NVP(connection_string) % config::attribute_optional();
        CONFIG_NVP(query);
    END_CONFIG_MAP()
};

struct root_cfg :public config::serializable {
    period_cfg period;
    db_cfg db;

    BEGIN_CONFIG_MAP()
        CONFIG_AGGR(period);
        CONFIG_AGGR(db);
    END_CONFIG_MAP()
};

}; //namespace

int print_config_struct(int argc, _TCHAR* argv[])
{
    tree::ptr viewer = release_factory<tree>::create_instance(L"root");
    root_cfg cfg_struct;
    cfg_struct.serialize(viewer);  // строим дерево
    print(viewer, 0); // печатаем дерево

    char c;
    std::cin >> c;
    return 0;
}

