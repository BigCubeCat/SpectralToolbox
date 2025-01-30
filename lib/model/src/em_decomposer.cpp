//
// Created by anton on 1/31/25.
//

#include "../include/em_decomposer.hpp"

#include "omp.h"
#include "segyreader.hpp"
#include "segywriter.hpp"

em_decomposer::em_decomposer(
    int max_iterations, int max_modes_cnt, int min_oscillation
) {
    m_decomposer.m_max_iterations  = max_iterations;
    m_decomposer.m_max_result_size = max_modes_cnt;
    m_decomposer.m_min_oscillation = min_oscillation;
}


std::vector<float_trace>
em_decomposer::examine(const std::string &path, long index) {
    segy_reader reader { path };
    float_trace trace = reader.trace(index);
    return m_decomposer.decompose_signal(trace);
}


void em_decomposer::decompose(
    const std::string &input_file,
    const std::string &output_dir,
    int red,
    int green,
    int blue
) {
    int cnt_threads = 8;
    // открываем файл на чтение и 3 файла на запись
    segy_reader reader { input_file };
    char *binheader = reader.binheader();
    int sub_start = input_file.rfind('/') + 1;
    int sub_end   = input_file.rfind('.');
    std::string output_filename =
        input_file.substr(sub_start, sub_end - sub_start);
    std::string red_file   = output_dir + "/" + output_filename + "_r.segy";
    std::string green_file = output_dir + "/" + output_filename + "_g.segy";
    std::string blue_file  = output_dir + "/" + output_filename + "_b.segy";
    segy_writer writer_red { red_file };
    writer_red.write_binheader(binheader);
    segy_writer writer_green { green_file };
    writer_green.write_binheader(binheader);
    segy_writer writer_blue { blue_file };
    writer_blue.write_binheader(binheader);

    // берем границы кросслайна для цикла
    int min_crossline = reader.min_crossline();
    int max_crossline = reader.max_crossline();

    std::vector<segy_reader> readers;
    for (int i = 0; i < cnt_threads; ++i) {
        readers.emplace_back(input_file);
    }

#pragma omp parallel for num_threads(cnt_threads)
    for (int i = min_crossline; i < max_crossline; i++) {
        // берем индексы разреза
        std::vector<int> idxs =
            readers[omp_get_thread_num()].get_crossline_layer(i);

        // для каждого разреза делаем декомпозицию
        for (auto &j : idxs) {
            float_trace trace = readers[omp_get_thread_num()].trace(j);

            std::vector<float_trace> results = m_decomposer.decompose_signal(trace);

            // сохраняем в файл
#pragma omp critical(red)
            writer_red.write_trace(j, results[0]);
#pragma omp critical(green)
            writer_green.write_trace(j, results[1]);
#pragma omp critical(blue)
            writer_blue.write_trace(j, results[2]);
        }
    }
}