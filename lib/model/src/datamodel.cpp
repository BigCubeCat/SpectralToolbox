#include "datamodel.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "segyreader.hpp"


proc_config_iface *datamodel::get_params() {
    return m_config.get();
}

void datamodel::open_file(const std::string &filename) {
    spdlog::info("filename={}", filename);
    m_reader_mutex.lock();
    // TODO: сделать фабрику для других типов
    m_data_reader = std::make_shared<segy_reader>(filename);
    m_reader_mutex.unlock();
}

reader_iface *datamodel::reader() {
    m_reader_mutex.lock();
    return m_data_reader.get();
}

void datamodel::unlock_reader() {
    m_reader_mutex.unlock();
}
