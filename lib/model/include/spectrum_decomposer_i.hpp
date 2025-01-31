//
// Created by anton on 1/31/25.
//

#ifndef SPECTRUM_DECOMPOSER_I_HPP
#define SPECTRUM_DECOMPOSER_I_HPP
#include <vector>

#include "trace.hpp"


class spectrum_decomposer_i {
public:
    virtual ~spectrum_decomposer_i() = default;
    /*!
     * @brief разбивает выбранную трассу на компоненты
     * @param path путь до файла с кубом данных
     * @param index индекс трассы в кубе данных
     * @return вектор выделенных компонеет
     */
    virtual std::vector<float_trace> examine(const std::string &path, long index) = 0;

    /*!
     * @brief выделяет из трасс куба 3  заданные компоненты. Файлы с
     * результатами сохраняются в заданную директорию с именем входного файла и
     * соответствующими суффиксами: _r _g _b
     * @param input_file путь до файла входных данных
     * @param output_dir путь до директории с результатами
     * @param red частота для красного цвета
     * @param green частота для зеленого цвета
     * @param blue частота для синего цвета
     */
    virtual void decompose(
        const std::string &input_file,
        const std::string &output_dir,
        int red,
        int green,
        int blue
    ) = 0;
};


#endif    // SPECTRUM_DECOMPOSER_I_HPP
