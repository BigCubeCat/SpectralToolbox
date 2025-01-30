//
// Created by anton on 1/30/25.
//

#ifndef EXTREMUM_SEARCHER_IMPL_HPP
#define EXTREMUM_SEARCHER_IMPL_HPP
#include "../include/extremum_searcher.h"
#include "omp.h"

using namespace emd;


template <typename E>
std::vector<int> extremum_searcher<E>::search_maxima(
    const Eigen::Vector<E, Eigen::Dynamic> &f_ys,
    bool check_borders,
    const int &cnt_threads
) {
    std::vector<std::vector<int>> maxima {};
    maxima.resize(cnt_threads);

#pragma omp parallel for num_threads(cnt_threads)
    for (int i = 1; i < f_ys.size() - 1; ++i) {
        if (f_ys[i - 1] < f_ys[i] && f_ys[i + 1] < f_ys[i]) {
            maxima[omp_get_thread_num()].push_back(i);
        }
    }
    std::vector<int> result {};

    for (auto &i : maxima) {
        result.insert(result.end(), i.begin(), i.end());
    }

    return result;
}


template <typename E>
std::vector<int> extremum_searcher<E>::search_minima(
    const Eigen::Vector<E, Eigen::Dynamic> &f_ys,
    bool check_borders,
    const int &cnt_threads
) {
    std::vector<std::vector<int>> minima {};
    minima.resize(cnt_threads);

#pragma omp parallel for num_threads(cnt_threads)
    for (int i = 1; i < f_ys.size() - 1; ++i) {
        if (f_ys[i - 1] > f_ys[i] && f_ys[i + 1] > f_ys[i]) {
            minima[omp_get_thread_num()].push_back(i);
        }
    }
    std::vector<int> result {};

    for (auto &i : minima) {
        result.insert(result.end(), i.begin(), i.end());
    }

    return result;
}




#endif //EXTREMUM_SEARCHER_IMPL_HPP
