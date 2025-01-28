#include "segyreader.hpp"

#include <stdexcept>
#include <string>

#include <segyio/segy.h>
#include <spdlog/spdlog.h>


segy_reader::segy_reader(std::string filename)
    : m_input_file_name(std::move(filename)),
      m_segy_file_ptr(segy_open(m_input_file_name.c_str(), "r")) {
    if (m_segy_file_ptr == nullptr) {
        spdlog::critical("segy file is nullptr");
        throw std::runtime_error("cant read segy file");
    }
    auto status = segy_binheader(m_segy_file_ptr, m_binheader.data());
    if (status != 0) {
        spdlog::critical("cant read binheader: status={}", status);
        throw std::runtime_error("cant read binheader");
    }
    m_format  = segy_format(m_binheader.data());
    m_samples = segy_samples(m_binheader.data());
    m_trace0  = segy_trace0(m_binheader.data());
    m_bsize   = segy_trace_bsize(m_samples);
    status    = segy_traces(m_segy_file_ptr, &m_traces, m_trace0, m_bsize);
}

char *segy_reader::binheader() {
    return m_binheader.data();
}

int segy_reader::count_traces() {
    return m_traces;
}

float_trace segy_reader::trace(int index) {
    Eigen::Vector<float, Eigen::Dynamic> trace(m_samples);
    auto err =
        segy_readtrace(m_segy_file_ptr, index, trace.data(), m_trace0, m_bsize);
    if (err != 0) {
        throw(std::runtime_error(
            "cant read trace no: " + std::to_string(index)
            + "; segyio status: " + std::to_string(err)
        ));
    }
    if (err = segy_to_native(m_format, m_samples, trace.data()); err != 0) {
        throw(std::runtime_error(
            "cant do native trace no: " + std::to_string(index)
            + "; segyio status: " + std::to_string(err)
        ));
    }
    return trace;
}

std::vector<int32_t> segy_reader::traceheader(int index) {
    std::vector<int32_t> result;
    result.resize(SEGY_TRACE_HEADER_SIZE / sizeof(int32_t));
    std::vector<char> data(SEGY_TRACE_HEADER_SIZE);
    auto status = segy_traceheader(
        m_segy_file_ptr, index, data.data(), m_trace0, m_bsize
    );
    if (status != 0) {
        throw std::runtime_error(
            "cant read traceheader: traceno={}" + std::to_string(index)
        );
    }
    result.assign(data.data());
    return result;
}

segy_reader::~segy_reader() {
    auto status = segy_close(m_segy_file_ptr);
    if (status != 0) {
        spdlog::critical("error during segy_close: status={}", status);
    }
}
