#include "datamodel.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "segyreader.hpp"


void *decomposer_routine(void *_unused) {
    auto *model = datamodel::instance();
    model->calculation_in_process.store(true);
    model->calculation_is_done.store(false);
    em_decomposer decomposer(5, 3, 0);
    std::string input  = model->get_string("in");
    std::string output = model->get_string("out");
    decomposer.decompose(input, output, 100, 50, 10);
    model->calculation_in_process.store(false);
    model->calculation_is_done.store(true);
    return nullptr;
}

void datamodel::open_file(const std::string &filename) {
    m_config.set_string("in", filename);
    spdlog::info("filename={}", filename);
    m_reader_mutex.lock();
    // TODO: сделать фабрику для других типов
    spdlog::info("init reader");
    m_data_reader = std::make_shared<segy_reader>(filename);
    spdlog::info("reader created");
    m_reader_mutex.unlock();
}

reader_iface *datamodel::reader() {
    m_reader_mutex.lock();
    return m_data_reader.get();
}

void datamodel::unlock_reader() {
    m_reader_mutex.unlock();
}

void datamodel::start_calculation() {
    std::thread t(decomposer_routine, nullptr);
    t.detach();
}
