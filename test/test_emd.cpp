//
// Created by anton on 1/31/25.
//
#include <gtest/gtest.h>
#include "em_decomposer.hpp"

TEST(emd_tests, analyse_cube_test) {
    em_decomposer decomposer(5,3,0);
    std::string input = "/home/anton/Downloads/seis.sgy";
    std::string output = "/home/anton/Downloads";
    decomposer.decompose(input, output, 100, 50 , 10);
}