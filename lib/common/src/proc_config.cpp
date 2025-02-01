#include "proc_config.hpp"

#include <stdexcept>

void proc_config::set_int(const std::string &key, int value) {
    m_int_map[key] = value;
}


void proc_config::set_float(const std::string &key, float value) {
    m_float_map[key] = value;
}

void proc_config::set_string(const std::string &key, std::string value) {
    m_string_map[key] = std::move(value);
}

std::string proc_config::get_string(const std::string &key) {
    auto value = m_string_map.find(key);
    if (value == m_string_map.end()) {
        throw std::runtime_error("not found " + key);
    }
    return value->second;
}

int proc_config::get_int(const std::string &key) {
    auto value = m_int_map.find(key);
    if (value == m_int_map.end()) {
        throw std::runtime_error("not found " + key);
    }
    return value->second;
}

float proc_config::get_float(const std::string &key) {
    auto value = m_float_map.find(key);
    if (value == m_float_map.end()) {
        throw std::runtime_error("not found " + key);
    }
    return value->second;
}
