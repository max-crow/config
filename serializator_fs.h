#pragma once
#include "serializator.h"
#include <boost/filesystem.hpp>

        namespace config {

            template <>
            class default_serializator<boost::filesystem::wpath> {
            public:
                static std::wstring to_string(boost::filesystem::wpath const& value) {
                    return value.native_file_string();
                    //return value?L"true":L"false";
                }
                static void parse(boost::filesystem::wpath& dest, std::wstring const& src) {
                    dest = boost::filesystem::wpath(src, boost::filesystem::native);
                    //dest = L"true" == src || L"yes" == src;
                }
            };

        } // namespace config
