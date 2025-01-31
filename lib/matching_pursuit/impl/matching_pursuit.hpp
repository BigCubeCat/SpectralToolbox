#pragma once
#include <vector>
#include <cmath>
#include <fstream>

#include "matching_pursuit.h"

template<typename T>
matrix matching_pursuit<T>::calc_correlate_matrix(trace& trace, matrix& dict){
    matrix correlate_matrix(dict.rows(), trace.rows() + dict.cols() - 1);
    correlate_matrix.setZero();

    Eigen::Index n_left  = dict.cols() - 1;
    Eigen::Index n_right = dict.cols() - 1;
    Eigen::Index rows = dict.rows();
    Eigen::Index trace_size = trace.rows();
    Eigen::Index filter_size = dict.cols();

#pragma omp parallel for
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < n_left; ++j){
            correlate_matrix(i, j) = trace.head(j+1).dot(dict.row(i).tail(j+1));
        }
        for (int j = 0; j < trace_size - filter_size + 1; j++){
            correlate_matrix(i, n_left + j) = trace.segment(j, filter_size).dot(dict.row(i));
        }
        for (int j = 0; j < n_right; ++j){
            correlate_matrix(i, j + trace_size) = trace.tail(filter_size - (j+1)).dot(dict.row(i).head(filter_size - (j+1)));
        }
    }
    return correlate_matrix;
}

template<typename T>
trace matching_pursuit<T>::get_axis(Eigen::MatrixXf& dict, int trace_size, int idx, int shift){
    trace axis(trace_size);
    axis.setZero();
    if (shift < dict.cols()){
        int start_idx = dict.cols() - shift;
        for (int j = 0; j < shift; ++j){
            axis(j) = dict(idx, j + start_idx);
        }
    } else {
        int start_idx = shift - dict.cols();
        for (int j = 0; (j < dict.cols()) && (j + start_idx < trace_size); ++j){
            axis(j + start_idx) = dict(idx, j); 
        }
    } 
    return axis;
}

template<typename T>
matrix matching_pursuit<T>::get_dict(int wavelet_length, float step_time_s){
    matrix dict(101, wavelet_length);
    for (int i = 0; i < 101; ++i){
        for (int j = 0; j < wavelet_length; ++j){
            float t = step_time_s * (j - wavelet_length/ 2);
            dict(i, j) = (1.0 - 2.0 * M_PI * M_PI * static_cast<float>(i * i * t * t)) 
            * std::exp(-M_PI * M_PI * static_cast<float>(i * i * t * t));
        }
        dict.row(i) /= dict.row(i).norm();
    }
    return dict;
}
    
std::vector<trace> matching_pursuit<T>::decompose_signal(float_trace gather, int trace_size, T step_time_s, T max_amplitude){

    std::vector<std::pair<int, int>> S;
    matrix dict = get_dict(100, step_time_s);
    trace r(trace_size);
    matrix correlate_matrix;
    int id_max_i, id_max_j;
    r = gather / gather.cwiseAbs().maxCoeff(&id_max_i);

    while(1) {
        correlate_matrix = calc_correlate_matrix(r, dict);
        correlate_matrix.cwiseAbs().maxCoeff(&id_max_i, &id_max_j);
        float amp = correlate_matrix(id_max_i, id_max_j);
        if ((r.norm() < 0.01) || (std::abs(amp) < max_amplitude)){
            break;
        }
        S.push_back(std::pair{id_max_i, id_max_j + 1});
        r = r - amp * get_axis(dict, trace_size, id_max_i, id_max_j + 1);
    }

    std::vector<trace> spectrum;
    for (int i =  0; i < dict.rows(); ++i){
        spectrum.push_back(trace(trace_size));
        spectrum[i].setZero();
    }
    
    for (const auto& s : S){
        trace axis = get_axis(dict, trace_size, S.first, s.second);
        for (int i = 0; i < trace_size; ++i){
            spectrum[s.first][j] = axis[j];
        }
    }
    return spectrum;
}
