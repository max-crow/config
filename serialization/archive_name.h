#pragma once

namespace config {


struct config_name {
    std::wstring& value;

    config_name(std::wstring& value) 
        :value(value) {
    }

};

#define CONFIG_SERIALIZATION_NAME_(ref_to_variable) \
    config::config_name(ref_to_variable)


}
