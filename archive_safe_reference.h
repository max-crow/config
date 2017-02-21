//#pragma once
//
//#include "config.h"
//
//namespace config {
//
//
//struct archive_safe_reference
//{
//    safe_element_reference_ptr& value;
//
//    archive_safe_reference(safe_element_reference_ptr& value)
//        :value(value) {
//    }
//
//};
//
//#define CONFIG_SERIALIZATION_CONFIG(ref_to_variable)                \
//    config::archive_safe_reference(ref_to_variable)
//
//}