//
// Created by anton on 1/31/25.
//

#include "../include/em_decomposer.hpp"
#include "segyreader.hpp"
#include "segywriter.hpp"
#include "omp.h"

em_decomposer::em_decomposer(
    int max_iterations, int max_modes_cnt, int min_oscillation
) {
    m_decomposer.m_max_iterations = max_iterations;
    m_decomposer.m_max_result_size = max_modes_cnt;
    m_decomposer.m_min_oscillation = min_oscillation;
}
std::vector<float_trace> em_decomposer::examine(const std::string &path, long index) {
    segy_reader reader{path};
    float_trace trace = reader.trace(index);
    return m_decomposer.decompose_signal(trace);
}
void em_decomposer::decompose(
    const std::string &input_file, const std::string &output_dir, int red, int green, int blue
) {
    segy_reader reader{input_file};
    std::string red_file = output_dir + "/" + std::to_string(red) + "_r.segy";
    segy_writer writer_red{output_dir};

}