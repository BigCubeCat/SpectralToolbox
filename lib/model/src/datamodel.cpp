#include "datamodel.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "em_decomposer.hpp"
#include "segyreader.hpp"


void *decomposer_routine(void *_unused) {
    auto *model = datamodel::instance();
    model->calculation_in_process.store(true);
    model->calculation_is_done.store(false);
    em_decomposer decomposer(5, 3, 0);
    std::string input  = model->get_string("in");
    std::string output = model->get_string("out");

    auto sub_start       = input.rfind('/') + 1;
    auto sub_end         = input.rfind('.');
    auto output_filename = input.substr(sub_start, sub_end - sub_start);

    std::string red_file   = output + "/" + output_filename + "_r.sgy";
    std::string green_file = output + "/" + output_filename + "_g.sgy";
    std::string blue_file  = output + "/" + output_filename + "_b.sgy";

    decomposer.setup(red_file, green_file, blue_file);

    decomposer.decompose(input, output, 100, 50, 10);

    model->open_result(red_file, green_file, blue_file);
    model->calculation_in_process.store(false);
    model->calculation_is_done.store(true);

    spdlog::info("routine end");

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

void datamodel::open_result(
    const std::string &red, const std::string &green, const std::string &blue
) {
    m_reader_mutex.lock();
    m_red_reader   = std::make_shared<segy_reader>(red);
    m_green_reader = std::make_shared<segy_reader>(green);
    m_blue_reader  = std::make_shared<segy_reader>(blue);
    calculation_is_done.store(true);
    m_reader_mutex.unlock();
}

reader_iface *datamodel::red_reader() {
    return m_red_reader.get();
}

reader_iface *datamodel::green_reader() {
    return m_green_reader.get();
}

reader_iface *datamodel::blue_reader() {
    return m_blue_reader.get();
}

void datamodel::unlock_reader() {
    m_reader_mutex.unlock();
}

void datamodel::start_calculation() {
    std::thread t(decomposer_routine, nullptr);
    t.detach();
}
