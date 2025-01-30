//
// Created by anton on 1/30/25.
//

#ifndef DECOMPOSER_H
#define DECOMPOSER_H
#include <vector>
#include <eigen3/Eigen/Dense>


namespace emd {

template <typename E>
class decomposer {
public:
    std::vector<Eigen::Vector<E, Eigen::Dynamic>>
    decompose_signal(const Eigen::Vector<E, Eigen::Dynamic> &trace);


    int m_max_iterations    = 5;    // limit of iterations
    int m_max_result_size   = 5;    // limit of modes set
    E m_min_oscillation = 1;    // if the rest oscillation < min_oscillation,\
                                     then algorithm stops
};


}    // namespace emd

#include "../src/decomposer_impl.hpp"

#endif    // DECOMPOSER_H