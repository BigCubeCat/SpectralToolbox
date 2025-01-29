#pragma once

#include <atomic>
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
    std::atomic<int> m_progress;
    std::shared_ptr<reader_iface> m_data_reader;
    std::shared_ptr<proc_config_iface> m_config;

    datamodel() {
        m_progress.store(0);
    }

public:
    datamodel &instance();

    proc_config_iface *get_params();

    /// открыть новый файл с данными
    void open_file(const std::string &filename);
};
