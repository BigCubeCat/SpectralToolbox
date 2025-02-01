//
// Created by anton on 2/1/25.
//
#include <gtest/gtest.h>
#include "idle_decomposer.hpp"

TEST(idle_tests, analyse_cube_test) {
    idle_decomposer decomposer{};
    std::string input = "/home/anton/Downloads/seis.sgy";
    std::string output = "/home/anton/Downloads/";
    std::string output_r = "/home/anton/Downloads/seis_r.sgy";
    std::string output_g = "/home/anton/Downloads/seis_g.sgy";
    std::string output_b = "/home/anton/Downloads/seis_b.sgy";
    decomposer.setup(output_r, output_g, output_b);
    decomposer.decompose(input, output,10,20,30);
}