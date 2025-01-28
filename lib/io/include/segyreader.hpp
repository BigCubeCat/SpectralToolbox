#pragma once

#include <array>
#include <string>

#include <segyio/segy.h>

#include <Eigen/Dense>

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
    long m_trace0;
    std::string m_input_file_name;

    std::array<char, SEGY_BINARY_HEADER_SIZE> m_binheader {};

    segy_file *m_segy_file_ptr = nullptr;

public:
    explicit segy_reader(std::string filename);
    ~segy_reader() override;

    char *binheader() override;

    /// возращает число трас в файле
    int count_traces() override;

    /// возращает вектор с трассой
    float_trace trace(int index) override;
    /// возращает traceheader
    std::vector<char> traceheader(int index) override;
};
