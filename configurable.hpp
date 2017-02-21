#pragma once
#include "config/config.h"
#include "config/failure.hpp"

    namespace config {
        class configurable2 {
        public:
            virtual ~configurable2() {}

            //Декларируем конфигурацию
            //virtual void configure_declare() const throw() = 0;
            //Только проверяем конфигурацию. В случае, если конфигурация валидна - возращаем истину
            //virtual bool configure_validate(config const& config_value) const throw() = 0;

            //Произвести конфигурирование модуля
            //virtual void configure(config const& config_value) throw(failure) = 0;
            virtual config::serializable& get_configure_class() = 0;

            //Вызывается после конфигурения модуля. В случае ошибки конфигурации, возращает false
            virtual bool post_configure() = 0;
        };
    } // namespace config
