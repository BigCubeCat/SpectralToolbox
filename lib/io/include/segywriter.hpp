#pragma once

#include <climits>
#include <string>

#include <segyio/segy.h>

#include <Eigen/Dense>

#include "writer.hpp"


/*!
 * \brief объект который читает sgy файл
 */
class segy_writer : public writer_iface {
private:
    std::string m_filename;
    segy_file *m_segy_file_ptr = nullptr;
    int m_samples              = 0;
    int m_bsize                = 0;
    int m_format               = 5;
    int m_traces               = 0;
    long m_trace0              = 0;

public:
    explicit segy_writer(std::string filename);
    ~segy_writer() override;

    void write_binheader(char *binheader) override;
    void write_trace(int index, float_trace trace) override;
    void write_traceheader(int index, std::vector<char> traceheader) override;
};
