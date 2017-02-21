// config.cpp : Defines the exported functions for the DLL application.
//

#include "factory.h"

using namespace config;

__declspec(dllexport)
reader_ptr create_reader_instance(const std::wstring& file_name) {
    return factory::create_reader(file_name);
};
