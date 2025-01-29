//
// Created by anton on 1/30/25.
//

#include "../include/extremum_searcher.h"

using namespace emd;

std::vector<int> extremum_searcher::search_maxima(
    const float_trace &f_ys, const int &cnt_threads
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


std::vector<int> extremum_searcher::search_minima(
    const float_trace &f_ys, const int &cnt_threads
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