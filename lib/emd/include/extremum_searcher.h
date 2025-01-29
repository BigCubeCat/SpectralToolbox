//
// Created by anton on 1/30/25.
//

#ifndef EXTREMUM_SEARCHER_H
#define EXTREMUM_SEARCHER_H
#include <vector>

#include "trace.hpp"


namespace emd {

/*!
 * @brief searches all local extremes on trace it uses openMP for splitting
 * computation
 */
class extremum_searcher {
public:
    /*!
     * @brief finds all local maximums.
     * @param f_ys eigen vector that represents function
     * @param cnt_threads count openMP threads
     * @return vector of indices of local maximums
     */
    static std::vector<int>
    search_maxima(const float_trace &f_ys, const int &cnt_threads);

    /*!
     * @brief finds all local minimums.
     * @param f_ys eigen vector that represents function
     * @param cnt_threads count openMP threads
     * @return vector of indices of local minimums
     */
    static std::vector<int>
    search_minima(const float_trace &f_ys, const int &cnt_threads);
};

}    // namespace emd

#endif    // EXTREMUM_SEARCHER_H
