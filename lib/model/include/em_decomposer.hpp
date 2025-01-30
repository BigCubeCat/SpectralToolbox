//
// Created by anton on 1/31/25.
//

#ifndef EM_DECOMPOSER_HPP
#define EM_DECOMPOSER_HPP
#include "spectrum_decomposer_i.hpp"
#include "decomposer.h"


class em_decomposer: public spectrum_decomposer_i {
public:
    em_decomposer() = default;
    em_decomposer(int max_iterations, int max_modes_cnt, int min_oscillation);

    std::vector<float_trace> examine(const std::string &path, long index) override;

    void decompose(
        const std::string &input_file,
        const std::string &output_dir,
        int red,
        int green,
        int blue
    ) override;

private:
    decomposer<float> m_decomposer;
};



#endif //EM_DECOMPOSER_HPP
