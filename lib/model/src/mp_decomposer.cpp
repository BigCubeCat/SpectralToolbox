//
// Created by anton on 1/31/25.
//

#include "mp_decomposer.hpp"

#include <iostream>

#include "segyreader.hpp"
#include "segywriter.hpp"

mp_decomposer::mp_decomposer(float step_time_s, float max_amplitude)
    : m_step_time_s(m_step_time_s), m_max_amplitude(max_amplitude) { };


std::vector<float_trace>
mp_decomposer::examine(const std::string &path, long index) {
    segy_reader reader { path };
    float_trace trace = reader.trace(index);
    return m_decomposer.decompose_signal(
        trace, trace.size(), m_step_time_s, m_max_amplitude
    );
}


void mp_decomposer::decompose(
    const std::string &input_file,
    const std::string &output_dir,
    int red,
    int green,
    int blue
) {
    int cnt_threads = 16;
    // открываем файл на чтение и 3 файла на запись
    segy_reader reader { input_file };
    char *binheader = reader.binheader();
    int sub_start   = input_file.rfind('/') + 1;
    int sub_end     = input_file.rfind('.');
    std::string output_filename =
        input_file.substr(sub_start, sub_end - sub_start);
    std::string red_file   = output_dir + "/" + output_filename + "_r.sgy";
    std::string green_file = output_dir + "/" + output_filename + "_g.sgy";
    std::string blue_file  = output_dir + "/" + output_filename + "_b.sgy";
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

    for (int i = min_crossline; i <= max_crossline; ++i) {

        // берем индексы разреза
        std::vector<int> idxs = reader.get_crossline_layer(i);

        std::cout << i << " " << idxs.size() << std::endl;

        std::vector<float_trace> red_line_traces { idxs.size() };
        std::vector<float_trace> blue_line_traces { idxs.size() };
        std::vector<float_trace> green_line_traces { idxs.size() };
        std::vector<std::vector<char>> line_trace_headers { idxs.size() };
        // для каждого разреза делаем декомпозицию
#pragma omp parallel for schedule(dynamic) proc_bind(spread) num_threads(cnt_threads)
        for (int j = 0; j < idxs.size(); ++j) {
            line_trace_headers[j] =
                readers[omp_get_thread_num()].traceheader(idxs[j]);
            float_trace trace = readers[omp_get_thread_num()].trace(idxs[j]);
            std::vector<float_trace> results = m_decomposer.decompose_signal(
                trace, trace.size(), m_step_time_s, m_max_amplitude
            );
            red_line_traces[j]   = results[red];
            green_line_traces[j] = results[green];
            blue_line_traces[j]  = results[blue];
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