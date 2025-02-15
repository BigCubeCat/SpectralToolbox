//
// Created by anton on 1/31/25.
//

#include "../include/em_decomposer.hpp"

#include "datamodel.hpp"
#include "omp.h"
#include "segyreader.hpp"
#include "segywriter.hpp"

em_decomposer::em_decomposer(
    int max_iterations, int max_modes_cnt, float min_oscillation
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
    segy_writer writer_red { m_red };
    writer_red.write_binheader(binheader);
    segy_writer writer_green { m_green };
    writer_green.write_binheader(binheader);
    segy_writer writer_blue { m_blue };
    writer_blue.write_binheader(binheader);

    // берем границы кросслайна для цикла
    int min_crossline = reader.min_crossline();
    int max_crossline = reader.max_crossline();

    std::vector<segy_reader> readers {};
    readers.reserve(cnt_threads);
    for (int i = 0; i < cnt_threads; i++) {
        readers.emplace_back(input_file);
    }

    for (int i = min_crossline; i <= max_crossline; ++i) {
        // берем индексы разреза
        std::vector<int> idxs = reader.get_crossline_layer(i);

        std::vector<float_trace> red_line_traces { idxs.size() };
        std::vector<float_trace> blue_line_traces { idxs.size() };
        std::vector<float_trace> green_line_traces { idxs.size() };
        std::vector<std::vector<char>> line_trace_headers { idxs.size() };
        // для каждого разреза делаем декомпозицию
#pragma omp parallel for proc_bind(spread) num_threads(cnt_threads)
        for (int j = 0; j < idxs.size(); ++j) {
            line_trace_headers[j] =
                readers[omp_get_thread_num()].traceheader(idxs[j]);
            float_trace trace = readers[omp_get_thread_num()].trace(idxs[j]);
            std::vector<float_trace> results =
                m_decomposer.decompose_signal(trace);
            red_line_traces[j]   = results[0];
            green_line_traces[j] = results[1];
            blue_line_traces[j]  = results[2];
        }

        // сохраняем в файлы
#pragma omp parallel proc_bind(spread) num_threads(3)
        {
#pragma omp critical(red)
            {
                for (int j = 0; j < idxs.size(); ++j) {
                    writer_red.write_traceheader(
                        idxs[j], line_trace_headers[j]
                    );
                    writer_red.write_trace(idxs[j], red_line_traces[j]);
                }
            }

#pragma omp critical(green)
            {
                for (int j = 0; j < idxs.size(); ++j) {
                    writer_green.write_traceheader(
                        idxs[j], line_trace_headers[j]
                    );
                    writer_green.write_trace(idxs[j], green_line_traces[j]);
                }
            }

#pragma omp critical(blue)
            {
                for (int j = 0; j < idxs.size(); ++j) {
                    writer_blue.write_traceheader(
                        idxs[j], line_trace_headers[j]
                    );
                    writer_blue.write_trace(idxs[j], blue_line_traces[j]);
                }
            }
        }
    }
}

void em_decomposer::setup(
    const std::string &red, const std::string &green, const std::string &blue
) {
    m_red   = red;
    m_green = green;
    m_blue  = blue;
}
