#include "segyreader.hpp"

#include <algorithm>
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
    if (status != 0) {
        spdlog::critical("cant read traceno: status={}", status);
        throw std::runtime_error("cant read count traces");
    }
    m_trace_no = std::vector<std::pair<int, int32_t>>(m_traces);
    for (int i = 0; i < m_traces; ++i) {
        auto tr_header = traceheader(i);
        m_traceheaders.push_back(tr_header);
        int32_t field;
        status = segy_get_field(tr_header.data(), SEGY_TR_TRACE_ID, &field);
        if (status != 0) {
            spdlog::error("status={}", status);
        }
        m_trace_no[i] = std::pair<int, int32_t>(i, field);

        int32_t trace_crossline;
        int32_t trace_inline;
        status =
            segy_get_field(tr_header.data(), SEGY_TR_INLINE, &trace_inline);
        if (status != 0) {
            spdlog::error("status = {}", status);
        }
        status = segy_get_field(
            tr_header.data(), SEGY_TR_CROSSLINE, &trace_crossline
        );
        if (status != 0) {
            spdlog::error("status = {}", status);
        }
        m_max_inline    = std::max(trace_inline, m_max_inline);
        m_max_crossline = std::max(trace_crossline, m_max_crossline);

        m_min_inline    = std::min(trace_inline, m_min_inline);
        m_min_crossline = std::min(trace_crossline, m_min_crossline);

        spdlog::info("{};{}", trace_crossline, trace_inline);
        m_inlines.emplace_back(i, trace_inline);
        m_crosslines.emplace_back(i, trace_crossline);
    }
    std::ranges::sort(m_trace_no, [](auto &left, auto &right) {
        return left.second < right.second;
    });
}

char *segy_reader::binheader() {
    return m_binheader.data();
}

int segy_reader::count_traces() {
    return m_traces;
}

float_trace segy_reader::trace(int index) {
    Eigen::Vector<float, Eigen::Dynamic> trace(m_samples);
    int traceno = m_trace_no[index].first;
    auto err    = segy_readtrace(
        m_segy_file_ptr, traceno, trace.data(), m_trace0, m_bsize
    );
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

std::vector<char> segy_reader::traceheader(int index) {
    std::vector<char> data(SEGY_TRACE_HEADER_SIZE);
    auto status = segy_traceheader(
        m_segy_file_ptr, index, data.data(), m_trace0, m_bsize
    );
    if (status != 0) {
        throw std::runtime_error(
            "cant read traceheader: traceno={}" + std::to_string(index)
        );
    }
    return data;
}

float segy_reader::trace_point(int index, int layer) {
    return trace(index)[layer];
}

int32_t segy_reader::max_crossline() {
    return m_max_crossline;
}

int32_t segy_reader::max_inline() {
    return m_max_inline;
}

int32_t segy_reader::min_crossline() {
    return m_min_crossline;
}

int32_t segy_reader::min_inline() {
    return m_min_inline;
}

int32_t segy_reader::trace_inline(int index) {
    return m_inlines[m_trace_no[index].first].second;
}

int32_t segy_reader::trace_crossline(int index) {
    return m_crosslines[m_trace_no[index].first].second;
}

segy_reader::~segy_reader() {
    auto status = segy_close(m_segy_file_ptr);
    if (status != 0) {
        spdlog::critical("error during segy_close: status={}", status);
    }
}
