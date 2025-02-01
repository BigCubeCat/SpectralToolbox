#include "segywriter.hpp"

#include <stdexcept>

#include <segyio/segy.h>
#include <spdlog/spdlog.h>

segy_writer::segy_writer(std::string filename)
    : m_filename(std::move(filename)),
      m_segy_file_ptr(segy_open(m_filename.c_str(), "w")) { }

void segy_writer::write_binheader(char *binheader) {
    spdlog::debug("is file_ptr null? {}", m_segy_file_ptr == nullptr);
    auto status = segy_write_binheader(m_segy_file_ptr, binheader);
    if (status != 0) {
        spdlog::critical("write_binheader: status={}", status);
        throw std::runtime_error(
            "write_binheader: status=" + std::to_string(status)
        );
    }
    m_format  = segy_format(binheader);
    m_samples = segy_samples(binheader);
    m_trace0  = segy_trace0(binheader);
    m_bsize   = segy_trace_bsize(m_samples);
    status    = segy_traces(m_segy_file_ptr, &m_traces, m_trace0, m_bsize);
}

void segy_writer::write_trace(int index, float_trace trace) {
    int status = segy_from_native(m_format, m_samples, trace.data());
    if (status != 0) {
        spdlog::error(
            "invalid segy_from_native: status=" + std::to_string(status)
        );
    }
    status = segy_writetrace(
        m_segy_file_ptr, index, trace.data(), m_trace0, m_bsize
    );
    if (status != 0) {
        spdlog::error("segy_writetrace error: " + std::to_string(status));
    }
}

void segy_writer::write_traceheader(int index, std::vector<char> traceheader) {
    auto status = segy_write_traceheader(
        m_segy_file_ptr, index, traceheader.data(), m_trace0, m_bsize
    );
    if (status != 0) {
        spdlog::error("status = {}", status);
    }
}

segy_writer::~segy_writer() {
    auto status = segy_close(m_segy_file_ptr);
    if (status != 0) {
        spdlog::error("segy close status = {}", status);
    }
}
