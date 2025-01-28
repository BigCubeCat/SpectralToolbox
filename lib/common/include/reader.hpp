#pragma once


#include <cstdint>
#include <vector>

#include "trace.hpp"

/*!
 * \brief интерфейс для чтения. используется для получения файлов
 */
class reader_iface {
public:
    virtual ~reader_iface() = default;
    /// возращает binheader. SEG-Y only
    virtual char *binheader() = 0;

    /// возращает число трас в файле
    virtual int count_traces() = 0;

    /// возращает вектор с трассой
    virtual float_trace trace(int index) = 0;
    /// возращает traceheader
    virtual std::vector<int32_t> traceheader(int index) = 0;
};
