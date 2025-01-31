//
// Created by anton on 1/30/25.
//

#ifndef EXTREMUM_SEARCHER_H
#define EXTREMUM_SEARCHER_H
#include <vector>

#include <eigen3/Eigen/Dense>



namespace emd {

/*!
 * @brief searches all local extremes on trace it uses openMP for splitting
 * computation
 */
template <typename E>
class extremum_searcher {
public:
    /*!
     * @brief finds all local maximums.
     * @param f_ys eigen vector that represents function
     * @param check_borders if it is true, border points will be compared with
     * neighbours
     * @param cnt_threads count openMP threads
     * @return vector of indices of local maximums
     */
    static std::vector<int> search_maxima(
        const Eigen::Vector<E, Eigen::Dynamic> &f_ys,
        bool check_borders,
        const int &cnt_threads
    );

    /*!
     * @brief finds all local minimums.
     * @param f_ys eigen vector that represents function
     * @param check_borders if it is true, border points will be compared with
     * neighbours
     * @param cnt_threads count openMP threads
     * @return vector of indices of local minimums
     */
    static std::vector<int> search_minima(
        const Eigen::Vector<E, Eigen::Dynamic> &f_ys,
        bool check_borders,
        const int &cnt_threads
    );
};

}    // namespace emd

#include "../src/extremum_searcher_impl.hpp"

#endif    // EXTREMUM_SEARCHER_H
