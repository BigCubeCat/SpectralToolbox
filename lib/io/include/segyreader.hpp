#pragma once

#include <array>
#include <climits>
#include <map>
#include <string>

#include <segyio/segy.h>

#include <eigen3/Eigen/Dense>

#include "reader.hpp"


/*!
 * \brief объект который читает sgy файл
 */
class segy_reader : public reader_iface {
private:
    int m_format;
    int m_bsize;
    int m_samples;
    int m_traces = 0;

    int m_max_inline    = 0;
    int m_max_crossline = 0;
    int m_min_inline    = INT_MAX;
    int m_min_crossline = INT_MAX;

    long m_trace0;
    std::string m_input_file_name;

    std::array<char, SEGY_BINARY_HEADER_SIZE> m_binheader {};

    std::vector<int32_t> m_trace_no;
    std::vector<std::pair<int, int32_t>> m_crosslines;
    std::vector<std::pair<int, int32_t>> m_inlines;
    std::vector<std::vector<char>> m_traceheaders;

    std::map<int, std::vector<int>> m_layers;

    segy_file *m_segy_file_ptr = nullptr;

public:
    explicit segy_reader(std::string filename);
    segy_reader(const segy_reader &) = default;
    segy_reader(segy_reader &&) = default;
    ~segy_reader() override;

    char *binheader() override;

    /// возращает число трас в файле
    int count_traces() override;

    int32_t max_crossline() override;
    int32_t max_inline() override;

    int32_t min_crossline() override;
    int32_t min_inline() override;

    /// возращает вектор с трассой
    float_trace trace(int index) override;

    float trace_point(int index, int layer) override;
    /// возращает traceheader
    std::vector<char> traceheader(int index) override;

    int32_t trace_inline(int index) override;

    int32_t trace_crossline(int index) override;
    int32_t trace_id(int index) override;

    std::vector<int> get_crossline_layer(int32_t crossline) override;
};
