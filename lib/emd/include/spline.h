//
// Created by anton on 1/30/25.
//

#ifndef SPLINE_H
#define SPLINE_H

#include <Eigen/Dense>

#include "trace.hpp"

using q_matrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

namespace emd {

/*!
 * @brief cubic spline
 */
class spline {
public:
    /*!
     * @brief computes quotients for cubic spline with algorithm from
     * http://www.ict.nsc.ru/matmod/files/textbooks/SharyNuMeth.pdf
     * @param xs x coordinates of basic points for spline
     * @param ys y coordinates of basic points for spline
     */
    spline(float_trace xs, float_trace ys);

    /*!
     * @brief computes values of spline function for given grid
     * @param xs grid
     * @return vector of spline values in grid points
     */
    float_trace compute(float_trace &xs) const;

private:
    /*!
     * @brief run-through method of searching matrix equal A*x = r solutions
     * @param matrix matix A of coefficients in A*x = r
     * @param right_part vector r in A*x = r
     * @return vector x in A*x = r
     */
    static float_trace
    m_run_through_method(const q_matrix &matrix, float_trace &right_part);

    float_trace m_gammas;
    float_trace m_ys;
    float_trace m_xs;
};

}    // namespace emd

#endif    // SPLINE_H
