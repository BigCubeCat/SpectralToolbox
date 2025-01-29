#pragma once

#include <memory>

#include "proc_config_iface.hpp"
#include "reader.hpp"

/*!
 * \brief Объект, хранящий все данные и являющийся промежуточным звеном между
 * View и данными
 * Является одиночкой
 */
class datamodel {
private:
    /// число от 0 до 100, прогресс в процентах
    int m_progress = 0;
    std::mutex m_reader_mutex;
    std::shared_ptr<reader_iface> m_data_reader;
    std::shared_ptr<proc_config_iface> m_config;

    datamodel() = default;

public:
    static datamodel *instance() {
        static datamodel inst;
        return &inst;
    }

    proc_config_iface *get_params();

    bool file_loaded() {
        return m_data_reader != nullptr;
    }

    /// открыть новый файл с данными
    void open_file(const std::string &filename);
};
