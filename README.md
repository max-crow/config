# config
A C++ serialization library to create, parse and visualize config files automatically – 
being once described with macros a structure stores its metadata, 
so it can be created and used as usual C++ structure but in the same time it can be serialized/deserialized automatically. 
Thus you can save your structure in XML, JSON, or any other format without any additional description.

## Example

A sample config:
```xml
<?xml version="1.0" encoding="utf-8"?>

<root>
  <period minutes="10" />
  <db connection_string="" query="">
</root>
```
This is how one could read this config from a file and use it in a c++ application:
```cpp
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

int main(int argc, _TCHAR* argv[])
{
    // Create an xml file reader
    config::config_ptr cfg_reader = config::xml_config_reader::load_from_file(L"c:\\sample_cfg.xml");

    root_cfg cfg;
    cfg.serialize(cfg_reader); // getting config from the file

    // We can use cfg as a usual c++ structure now:
    std::cout << cfg.db.query;

    return 0;
}
```

