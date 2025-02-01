#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "../include/matching_pursuit.h"

const int COUNT_ITERATIONS = 20;

template <typename T>
matching_pursuit<T>::matrix matching_pursuit<T>::calc_correlate_matrix(
    matching_pursuit<T>::trace &m_trace, matching_pursuit<T>::matrix &dict
) {
    matching_pursuit<T>::matrix correlate_matrix(
        dict.rows(), m_trace.rows() + dict.cols() - 1
    );
    correlate_matrix.setZero();

    Eigen::Index n_left      = dict.cols() - 1;
    Eigen::Index n_right     = dict.cols() - 1;
    Eigen::Index rows        = dict.rows();
    Eigen::Index trace_size  = m_trace.rows();
    Eigen::Index filter_size = dict.cols();


    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < n_left; ++j) {
            correlate_matrix(i, j) =
                m_trace.head(j + 1).dot(dict.row(i).tail(j + 1));
        }
        for (int j = 0; j < trace_size - filter_size + 1; j++) {
            correlate_matrix(i, n_left + j) =
                m_trace.segment(j, filter_size).dot(dict.row(i));
        }
        for (int j = 0; j < n_right; ++j) {
            correlate_matrix(i, j + trace_size) =
                m_trace.tail(filter_size - (j + 1))
                    .dot(dict.row(i).head(filter_size - (j + 1)));
        }
    }
    return correlate_matrix;
}

template <typename T>
matching_pursuit<T>::trace matching_pursuit<T>::get_axis(
    matching_pursuit<T>::matrix &dict, int trace_size, int idx, int shift
) {
    matching_pursuit<T>::trace axis(trace_size);
    axis.setZero();
    if (shift < dict.cols()) {
        int start_idx = dict.cols() - shift;
        for (int j = 0; j < shift; ++j) {
            axis(j) = dict(idx, j + start_idx);
        }
    }
    else {
        int start_idx = shift - dict.cols();
        for (int j = 0; (j < dict.cols()) && (j + start_idx < trace_size);
             ++j) {
            axis(j + start_idx) = dict(idx, j);
        }
    }
    return axis;
}

template <typename T>
matching_pursuit<T>::matrix
matching_pursuit<T>::get_dict(int wavelet_length, T step_time_s) {
    matching_pursuit<T>::matrix dict(101, wavelet_length);
    for (int i = 0; i < 101; ++i) {
        for (int j = 0; j < wavelet_length; ++j) {
            float t = step_time_s * (j - wavelet_length / 2);
            dict(i, j) =
                (1.0 - 2.0 * M_PI * M_PI * static_cast<T>(i * i * t * t))
                * std::exp(-M_PI * M_PI * static_cast<T>(i * i * t * t));
        }
        dict.row(i) /= dict.row(i).norm();
    }
    return dict;
}


template <typename T>
std::vector<Eigen::Vector<T, Eigen::Dynamic>>
matching_pursuit<T>::decompose_signal(
    matching_pursuit<T>::trace gather,
    int trace_size,
    T step_time_s,
    T max_amplitude
) {
    std::vector<std::pair<int, int>> s;
    matching_pursuit<T>::matrix dict = get_dict(100, step_time_s);
    matching_pursuit<T>::trace r(trace_size);
    matching_pursuit<T>::matrix correlate_matrix;
    int id_max_i;
    int id_max_j;
    r = gather / gather.cwiseAbs().maxCoeff(&id_max_i);
    s.reserve(COUNT_ITERATIONS);
    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        correlate_matrix = calc_correlate_matrix(r, dict);
        correlate_matrix.cwiseAbs().maxCoeff(&id_max_i, &id_max_j);
        float amp = correlate_matrix(id_max_i, id_max_j);

        s.emplace_back(id_max_i, id_max_j + 1);
        trace axis = get_axis(dict, trace_size, id_max_i, id_max_j);
        r          = r - amp * axis;
        if ((r.norm() < 0.1)) {
            break;
        }
    }

    std::vector<matching_pursuit<T>::trace> spectrum(dict.rows());
#pragma omp parallel for
    for (int i = 0; i < dict.rows(); ++i) {
        spectrum[i] = matching_pursuit<T>::trace(trace_size);
        spectrum[i].setZero();
    }

#pragma omp parallel for
    for (const auto &s : s) {
        matching_pursuit<T>::trace axis =
            get_axis(dict, trace_size, s.first, s.second);
        for (int i = 0; i < trace_size; ++i) {
            spectrum[s.first][i] = axis[i];
        }
    }
    return spectrum;
}
