//
// Created by anton on 1/30/25.
//

#ifndef SPLINE_IMPL_HPP
#define SPLINE_IMPL_HPP

#include <utility>

#include "../include/spline.h"

using namespace emd;
template <typename E>
spline<E>::spline(
    Eigen::Vector<E, Eigen::Dynamic> xs, Eigen::Vector<E, Eigen::Dynamic> ys
)
    : m_ys(ys), m_xs(xs) {

    // FILL THREE-DIAGONAL MATRIX
    Eigen::Matrix<E,Eigen::Dynamic,Eigen::Dynamic> coefficients(xs.size(), xs.size());
    coefficients.setZero();

    coefficients(0, 0) = 2 * (xs[1] - xs[0]);    // read the manual of splines
    coefficients(0, 1) = xs[1] - xs[0];

    for (long i = 1, end = coefficients.cols() - 1; i != end; ++i) {
        coefficients(i, i - 1) = xs[i] - xs[i - 1];
        coefficients(i, i)     = 2 * (xs[i + 1] - xs[i - 1]);
        coefficients(i, i + 1) = xs[i + 1] - xs[i];
    }

    coefficients(xs.size() - 1, xs.size() - 2) =
        xs[xs.size() - 1] - xs[xs.size() - 2];
    coefficients(xs.size() - 1, xs.size() - 1) =
        2 * (xs[xs.size() - 1] - xs[xs.size() - 2]);


    // FILL RIGHT-PART VECTOR

    E left_derived = (ys[1] - ys[0]) / (xs[1] - xs[0]);

    E right_derived = (ys[xs.size() - 1] - ys[xs.size() - 2])
                          / (xs[xs.size() - 1] - xs[xs.size() - 2]);

    Eigen::Vector<E, Eigen::Dynamic> right_part { xs.size() };

    right_part[0] = 6 * ((ys[1] - ys[0]) / (xs[1] - xs[0]) - left_derived);
    right_part[xs.size() - 1] =
        6
        * (right_derived
           - (ys[ys.size() - 1] - ys[ys.size() - 2])
                 / (xs[xs.size() - 1] - xs[xs.size() - 2]));

    for (long i = 1, end = right_part.rows() - 1; i != end; ++i) {
        right_part[i] = 6
                        * ((ys[i + 1] - ys[i]) / (xs[i + 1] - xs[i])
                           - (ys[i] - ys[i - 1]) / (xs[i] - xs[i - 1]));
    }

    // COMPUTE COEFFICIENTS WITH RUN_THROUGH METHOD
    m_gammas = m_run_through_method(coefficients, right_part);
}


template<typename E>Eigen::Vector<E,Eigen::Dynamic> spline<E>::compute(Eigen::Vector<E,Eigen::Dynamic> &xs) const {
    Eigen::Vector<E,Eigen::Dynamic> values { xs.size() };
    long cur = 0;

    // compute value for each x in vector xs
    for (long i = 0, end = xs.size(); i != end; ++i) {
        // x should be between 2 closest neighbour nodes of spline
        while (cur + 1 < m_xs.size() - 1 && m_xs[cur + 1] < xs[i])
            ++cur;

        E h_i1  = m_xs[cur + 1] - m_xs[cur];
        E f_dif = m_xs[cur + 1] - xs[i];
        E b_dif = xs[i] - m_xs[cur];
        values[i] =
            m_ys[cur] * f_dif / h_i1 +    // read the manual from header file
            m_ys[cur + 1] * b_dif / h_i1
            + m_gammas[cur] * (std::pow(f_dif, 3) - std::pow(h_i1, 2) * f_dif)
                  / (6 * h_i1)
            + m_gammas[cur + 1]
                  * (std::pow(b_dif, 3) - std::pow(h_i1, 2) * b_dif)
                  / (6 * h_i1);
    }
    return values;
}


template<typename E> Eigen::Vector<E, Eigen::Dynamic>
spline<E>::m_run_through_method(const Eigen::Matrix<E,Eigen::Dynamic,Eigen::Dynamic> &matrix, Eigen::Vector<E,Eigen::Dynamic> &right_part) {
    // INIT
    Eigen::Vector<E,Eigen::Dynamic> x_vector { right_part.size() };
    Eigen::Vector<E,Eigen::Dynamic> xi_vector { right_part.size() };
    Eigen::Vector<E,Eigen::Dynamic> eta_vector { right_part.size() };

    // MOVE FORWARD
    for (long i = 0, end = matrix.cols(); i < end; ++i) {
        if (i == 0) {
            xi_vector[0]  = -matrix(0, 1) / matrix(0, 0);
            eta_vector[0] = right_part[0] / matrix(0, 0);
        }
        else if (i == matrix.cols() - 1) {
            xi_vector[i] = 0;
            eta_vector[i] =
                (right_part[i] - matrix(i, i - 1) * eta_vector[i - 1])
                / (matrix(i, i - 1) * xi_vector[i - 1] + matrix(i, i));
        }
        else {
            xi_vector[i] =
                -matrix(i, i + 1)
                / (matrix(i - 1, i) * xi_vector[i - 1] + matrix(i, i));
            eta_vector[i] =
                (right_part[i] - matrix(i, i - 1) * eta_vector[i - 1])
                / (matrix(i, i - 1) * xi_vector[i - 1] + matrix(i, i));
        }
    }

    // MOVE BACK
    for (long i = matrix.cols() - 1; i >= 0; i--) {
        E next_x = 0;
        if (i < matrix.cols() - 1) {
            next_x = x_vector[i + 1];
        }
        x_vector[i] = xi_vector[i] * next_x + eta_vector[i];
    }

    return x_vector;
}

#endif    // SPLINE_IMPL_HPP
