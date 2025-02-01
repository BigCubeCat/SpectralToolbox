//
// Created by anton on 1/31/25.
//

#ifndef EM_DECOMPOSER_HPP
#define EM_DECOMPOSER_HPP
#include "decomposer.h"
#include "spectrum_decomposer_i.hpp"


class em_decomposer : public spectrum_decomposer_i {
public:
    em_decomposer() = default;
    em_decomposer(int max_iterations, int max_modes_cnt, float min_oscillation);

    std::vector<float_trace>
    examine(const std::string &path, long index) override;

    void decompose(
        const std::string &input_file,
        const std::string &output_dir,
        int red,
        int green,
        int blue
    ) override;

    void setup(
        const std::string &red,
        const std::string &green,
        const std::string &blue
    );

private:
    decomposer<float> m_decomposer;

    std::string m_red;
    std::string m_green;
    std::string m_blue;
};


#endif    // EM_DECOMPOSER_HPP
