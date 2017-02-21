#include "stdafx.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <map>

#include "core\config\config.h"


//struct period_cfg :public config::serializable {
//    int minutes;
//
//	BEGIN_CONFIG_MAP()
//        CONFIG_NVP(minutes);
//	END_CONFIG_MAP()
//}; 
//
namespace {

struct named_map_cfg :public config::serializable {  
    std::map<std::string, config::config_ptr> items;
    //std::string query;

	BEGIN_CONFIG_MAP()
        CONFIG_AGGR_NAMED_MAP(items);
    END_CONFIG_MAP()
};

struct root_cfg :public config::serializable {
    std::map<std::string, config::config_ptr> named_map;
    //db_cfg db;

    BEGIN_CONFIG_MAP()
        CONFIG_AGGR_NAMED_MAP(named_map);
        //CONFIG_AGGR(db);
    END_CONFIG_MAP()
};

}; //namespace

int simple_read(int argc, _TCHAR* argv[])
{
    //std::wstring config_name(argv[0]);
    std::wstring config_name = L"c:\\simple_read.xml";
    config::config_ptr cfg_reader = config::xml_config_reader::load_from_file(config_name);

    root_cfg cfg;
    cfg.serialize(cfg_reader);

    return 0;
}

