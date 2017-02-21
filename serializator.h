#pragma once

#include <stdlib.h>

#include <boost/lexical_cast.hpp> 

#include "core\utilities\string.hpp"

namespace config {

class cfg_locale {
public:
    cfg_locale() {
        value = _create_locale(LC_ALL, "english");
    }

    cfg_locale(_locale_t src) {
        value = src;
    }

    cfg_locale(cfg_locale& src) {
        value = src.value;
        src.value = NULL;
    }

    cfg_locale(int category, const char *locale) {
        value = _create_locale(category, locale);
    }
    
    ~cfg_locale() {
        if(value != NULL) {
            _free_locale(value);
        }
    }

    static cfg_locale get_default() {
        return cfg_locale();
    }

    operator _locale_t() {
        return value;
    }
private:
    _locale_t value;

    //static cfg_locale;
};

//template <class T>
//class serializable_wrapper :public T {
//};

template <class T>
class default_serializator
{
public:
    static std::wstring to_string(T& value) {
        // если тут выдается ошибка, то скорее всего
        // нужна специализация для используемого класса, 
        // либо где-то используется nvp вместо serialize()
        return boost::lexical_cast<std::wstring>(value);
        //return value;
    }
    static void parse(T& dest, const std::wstring& src) {
        // если тут выдается ошибка, то скорее всего
        // нужна специализация для используемого класса
        // либо где-то используется nvp вместо serialize()
        dest = boost::lexical_cast<T>(src);
    }
};

template <>
class default_serializator<std::wstring> {
public:
    typedef std::wstring T;
    static std::wstring to_string(T& value) {
        return value;
    }
    static void parse(T& dest, const std::wstring& src) {
        // если тут выдается ошибка, то скорее всего
        // нужна спецификация для использумого класса
        dest = src;
    }
};

template <>
class default_serializator<std::string> {
public:
    typedef std::string T;
    static std::wstring to_string(T& value) {
        return ::utilities::to_wstring(value);
    }
    static void parse(T& dest, const std::wstring& src) {
        dest = ::utilities::to_string(src);
    }
};

template <>
class default_serializator<bool> {
public:
    static std::wstring to_string(bool value) {
        return value?L"true":L"false";
    }
    static void parse(bool& dest, std::wstring const& src) {
        dest = L"true" == src || L"yes" == src;
    }
};
//template <>
//class default_serializator<int> {
//public:
//    typedef int T;
//    static std::wstring to_string(T& value) {
//        wchar_t str[10]= {0};
//        _itow_s(value, str, 10, 10);
//        return str;
//    }
//    static void parse(T& dest, const std::wstring& src) {
//        // если тут выдается ошибка, то скорее всего
//        // нужна спецификация для использумого класса
//        dest = _wtoi_l(src.c_str(), cfg_locale::get_default());
//    }
//};

//template <>
//class default_serializator<float> {
//    typedef float T;
//    static std::wstring to_string(T& value) {
//        wchar_t str[50]= {0};
//        _ecvt_s(value, str, 10, 10);
//        return str;
//    }
//    static void parse(T& dest, const std::wstring& src) {
//        dest = _wtof_l(src, cfg_locale::get_default());
//    }
//};

}
