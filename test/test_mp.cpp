//
// Created by anton on 1/31/25.
//
#include <gtest/gtest.h>
#include "mp_decomposer.hpp"

TEST(mp_tests, analyse_cube_test) {
    mp_decomposer decomposer(0.002, 1.5);
    std::string input = "/home/anton/Downloads/seis.sgy";
    std::string output = "/home/anton/Downloads";
    decomposer.decompose(input, output, 20, 30 , 40);
}