//
// Created by anton on 1/30/25.
//

#ifndef SPLINE_H
#define SPLINE_H

#include <eigen3/Eigen/Dense>


using q_matrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

namespace emd {

/*!
 * @brief cubic spline
 */
template <typename E>
class spline {
public:
    /*!
     * @brief computes quotients for cubic spline with algorithm from
     * http://www.ict.nsc.ru/matmod/files/textbooks/SharyNuMeth.pdf
     * @param xs x coordinates of basic points for spline
     * @param ys y coordinates of basic points for spline
     */
    spline(
        Eigen::Vector<E, Eigen::Dynamic> xs, Eigen::Vector<E, Eigen::Dynamic> ys
    );

    /*!
     * @brief computes values of spline function for given grid
     * @param xs grid
     * @return vector of spline values in grid points
     */
    Eigen::Vector<E, Eigen::Dynamic>
    compute(Eigen::Vector<E, Eigen::Dynamic> &xs) const;

private:
    /*!
     * @brief run-through method of searching matrix equal A*x = r solutions
     * @param matrix matix A of coefficients in A*x = r
     * @param right_part vector r in A*x = r
     * @return vector x in A*x = r
     */
    static Eigen::Vector<E, Eigen::Dynamic> m_run_through_method(
        const Eigen::Matrix<E, Eigen::Dynamic, Eigen::Dynamic> &matrix,
        Eigen::Vector<E, Eigen::Dynamic> &right_part
    );

    Eigen::Vector<E, Eigen::Dynamic> m_gammas;
    Eigen::Vector<E, Eigen::Dynamic> m_ys;
    Eigen::Vector<E, Eigen::Dynamic> m_xs;
};


}    // namespace emd

#include "../src/spline_impl.hpp"

#endif    // SPLINE_H
