#pragma once


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
    /// возращает значение трассы по слою
    virtual float trace_point(int index, int layer) = 0;
    /// возращает traceheader
    virtual std::vector<char> traceheader(int index) = 0;

    virtual int32_t max_crossline() = 0;
    virtual int32_t max_inline()    = 0;
    virtual int32_t min_crossline() = 0;
    virtual int32_t min_inline()    = 0;


    /// возращает TR_INLINE по номеру трассы
    virtual int32_t trace_inline(int index) = 0;
    /// возращает TR_CROSSLINE по номеру трассы
    virtual int32_t trace_crossline(int index) = 0;
};
