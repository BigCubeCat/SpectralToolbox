#pragma once

#include <string>

/*!
 * \brief интерфейс для параметров процедры
 * позволяет по имени вернуть или задать аргумент
 */
class proc_config_iface {
public:
    /// возращает строковое значение по ключу
    std::string get_string(const std::string &key);
    /// возращает целочисленное значение по ключу
    int get_int(const std::string &key);
    /// возращает float значение по ключу
    float get_float(const std::string &key);
    /// возращает double значение по ключу
    double get_double(const std::string &key);
    /// возращает bool значение по ключу
    bool get_bool(const std::string &key);

    /// устанавливает строковое значение по ключу
    void set_string(const std::string &key, std::string value);
    /// возращает целочисленное значение по ключу
    void set_int(const std::string &key, int value);
    /// возращает float значение по ключу
    void set_float(const std::string &key, float value);
    /// возращает double значение по ключу
    void set_double(const std::string &key, double value);
    /// устанаыливает bool значение по ключу
    void set_bool(const std::string &key, bool value);
};
