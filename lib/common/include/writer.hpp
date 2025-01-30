#pragma once

#include <vector>

#include "trace.hpp"

/*!
 * \brief интерфейс для чтения. используется для получения файлов
 */
class writer_iface {
public:
    virtual ~writer_iface() = default;
    /// возращает binheader. SEG-Y only
    virtual void write_binheader(char *binheader) = 0;
    /// возращает вектор с трассой
    virtual void write_trace(int index, float_trace trace) = 0;
    /// записывает traceheader
    virtual void write_traceheader(int index, std::vector<char>) = 0;
};
