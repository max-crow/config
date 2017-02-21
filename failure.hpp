#pragma once
#include <string>
#include <exception>

    namespace config {
        class failure: public std::exception {
        public:
            failure(std::string const& reason): std::exception(reason.c_str()) {}
            virtual ~failure() {}
        };

        class item_not_found: public failure {
        public:
            item_not_found(std::string const& item_name): failure("item '"+item_name+"' not found in config") {}
            virtual ~item_not_found() {}
        };

        class bad_cast: public failure {
        public:
            bad_cast(std::string const& reason): failure(reason) {}
            virtual ~bad_cast() {}
        };
    } // namespace config
