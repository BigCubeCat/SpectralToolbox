//
// Created by anton on 2/1/25.
//

#ifndef IDLE_DECOMPOSER_H
#define IDLE_DECOMPOSER_H
#include "spectrum_decomposer_i.hpp"


class idle_decomposer: public spectrum_decomposer_i {
public:
    idle_decomposer() = default;

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
    std::string m_red;
    std::string m_green;
    std::string m_blue;
};


#endif //IDLE_DECOMPOSER_H
