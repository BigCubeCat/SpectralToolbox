#pragma once

#include <memory>
#include <thread>
#include <utility>

#include "em_decomposer.hpp"
#include "proc_config.hpp"
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
    proc_config m_config;

    datamodel() = default;

public:
    std::atomic<bool> calculation_in_process;
    std::atomic<bool> calculation_is_done;

    static datamodel *instance() {
        static datamodel inst;
        return &inst;
    }

    bool file_loaded() {
        return m_data_reader != nullptr;
    }

    void start_calculation();

    /// открыть новый файл с данными
    void open_file(const std::string &filename);

    reader_iface *reader();

    void unlock_reader();

    /// устанавливает строковое значение по ключу
    void set_string(const std::string &key, std::string value) {
        m_config.set_string(key, std::move(value));
    }
    /// возращает целочисленное значение по ключу
    void set_int(const std::string &key, int value) {
        m_config.set_int(key, value);
    }
    /// возращает float значение по ключу
    void set_float(const std::string &key, float value) {
        m_config.set_float(key, value);
    }

    std::string get_string(const std::string &key) {
        return m_config.get_string(key);
    }
    int get_int(const std::string &key) {
        return m_config.get_int(key);
    }
    float get_float(const std::string &key) {
        return m_config.get_float(key);
    }
};
