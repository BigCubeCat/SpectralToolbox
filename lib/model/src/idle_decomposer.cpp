//
// Created by anton on 2/1/25.
//
#include "../include/idle_decomposer.hpp"

#include <filesystem>

#include "segyreader.hpp"


std::vector<float_trace>
idle_decomposer::examine(const std::string &path, long index) {
    std::vector<float_trace> traces;
    segy_reader reader { path };
    float_trace trace = reader.trace(index);
    traces.push_back(trace);
    return traces;
}
void idle_decomposer::decompose(
    const std::string &input_file,
    const std::string &output_dir,
    int red,
    int green,
    int blue
) {
    std::filesystem::path input_path { input_file };
    std::filesystem::path output_red_path { m_red };
    std::filesystem::path output_green_path { m_green };
    std::filesystem::path output_blue_path { m_blue };

    std::filesystem::copy_file(input_path, output_red_path, std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy_file(input_file, output_green_path, std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy_file(input_file, output_blue_path, std::filesystem::copy_options::overwrite_existing);
}


void idle_decomposer::setup(
    const std::string &red, const std::string &green, const std::string &blue
) {
    m_red   = red;
    m_green = green;
    m_blue  = blue;
 }