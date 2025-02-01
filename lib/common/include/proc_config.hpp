#pragma once

#include <string>
#include <unordered_map>

/*!
 * \brief интерфейс для параметров процедры
 * позволяет по имени вернуть или задать аргумент
 */
class proc_config {
private:
    std::unordered_map<std::string, std::string> m_string_map;
    std::unordered_map<std::string, int> m_int_map;
    std::unordered_map<std::string, float> m_float_map;

public:
    /// возращает строковое значение по ключу
    std::string get_string(const std::string &key);
    /// возращает целочисленное значение по ключу
    int get_int(const std::string &key);
    /// возращает float значение по ключу
    float get_float(const std::string &key);
    /// возращает double значение по ключу

    /// устанавливает строковое значение по ключу
    void set_string(const std::string &key, std::string value);
    /// возращает целочисленное значение по ключу
    void set_int(const std::string &key, int value);
    /// возращает float значение по ключу
    void set_float(const std::string &key, float value);
};
