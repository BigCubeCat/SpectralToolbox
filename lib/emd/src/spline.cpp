//
// Created by anton on 1/30/25.
//

#include "../include/spline.h"

#include <utility>

using namespace emd;
spline::spline(float_trace xs, float_trace ys)
    : m_xs(std::move(xs)), m_ys(std::move(ys)) {

    // FILL THREE-DIAGONAL MATRIX
    q_matrix coefficients(xs.size(), xs.size());
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

    float left_derived = (ys[1] - ys[0]) / (xs[1] - xs[0]);

    float right_derived = (ys[xs.size() - 1] - ys[xs.size() - 2])
                          / (xs[xs.size() - 1] - xs[xs.size() - 2]);

    float_trace right_part { xs.size() };

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
float_trace spline::compute(float_trace &xs) const {
    float_trace values { xs.size() };
    long cur = 0;

    //compute value for each x in vector xs
    for (long i = 0, end = xs.size(); i != end; ++i) {
        //x should be between 2 closest neighbour nodes of spline
        while (cur + 1 < m_xs.size() - 1 && m_xs[cur + 1] < xs[i])
            ++cur;

        float h_i1  = m_xs[cur + 1] - m_xs[cur];
        float f_dif = m_xs[cur + 1] - xs[i];
        float b_dif = xs[i] - m_xs[cur];
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


float_trace
spline::m_run_through_method(const q_matrix &matrix, float_trace &right_part) {
    // INIT
    float_trace x_vector { right_part.size() };
    float_trace xi_vector { right_part.size() };
    float_trace eta_vector { right_part.size() };

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
        float next_x = 0;
        if (i < matrix.cols() - 1) {
            next_x = x_vector[i + 1];
        }
        x_vector[i] = xi_vector[i] * next_x + eta_vector[i];
    }

    return x_vector;
}
