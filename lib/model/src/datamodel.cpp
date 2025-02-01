#include "datamodel.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "em_decomposer.hpp"
#include "mp_decomposer.hpp"
#include "routine_arg.hpp"
#include "segyreader.hpp"


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

void datamodel::start_calculation(e_decomposer comp) {
    auto *model = datamodel::instance();

    m_arg.input_file = model->get_string("in");
    m_arg.output_dir = model->get_string("out");

    auto sub_start = m_arg.input_file.rfind('/') + 1;
    auto sub_end   = m_arg.input_file.rfind('.');
    auto output_filename =
        m_arg.input_file.substr(sub_start, sub_end - sub_start);

    m_arg.red_file   = m_arg.output_dir + "/" + output_filename + "_r.sgy";
    m_arg.green_file = m_arg.output_dir + "/" + output_filename + "_g.sgy";
    m_arg.blue_file  = m_arg.output_dir + "/" + output_filename + "_b.sgy";

    m_arg.red   = 0;
    m_arg.green = 0;
    m_arg.blue  = 0;

    if (comp == EMD) {
        std::thread t(datamodel::emd_routine, &m_arg);
        t.detach();
    }
    else if (comp == MP) {
        std::thread t(datamodel::mp_routine, &m_arg);
        t.detach();
    }
}

void *datamodel::emd_routine(void *_unused) {
    auto *arg   = static_cast<routine_arg *>(_unused);
    auto *model = datamodel::instance();
    model->calculation_in_process.store(true);
    model->calculation_is_done.store(false);
    em_decomposer decomposer(5, 3, 0);
    decomposer.setup(arg->red_file, arg->green_file, arg->blue_file);
    decomposer.decompose(
        arg->input_file, arg->output_dir, arg->blue, arg->green, arg->red
    );
    model->open_result(arg->red_file, arg->green_file, arg->blue_file);
    model->calculation_in_process.store(false);
    model->calculation_is_done.store(true);
    spdlog::info("routine end");
    return nullptr;
}

void *datamodel::mp_routine(void *_unused) {
    auto *arg   = static_cast<routine_arg *>(_unused);
    auto *model = datamodel::instance();
    model->calculation_in_process.store(true);
    model->calculation_is_done.store(false);
    mp_decomposer decomposer(arg->time, arg->amp);
    decomposer.decompose(
        arg->input_file, arg->output_dir, arg->blue, arg->green, arg->red
    );
    model->open_result(arg->red_file, arg->green_file, arg->blue_file);
    model->calculation_in_process.store(false);
    model->calculation_is_done.store(true);
    spdlog::info("routine end");
    return nullptr;
}
