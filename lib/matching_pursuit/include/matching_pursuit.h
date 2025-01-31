#pragma once
#include <Eigen/Dense>

template<typename T>
class matching_pursuit {

    using trace = Eigen::Vector<T, Eigen::Dynamic>;
    using matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    private:
    matrix calc_correlate_matrix(trace& trace,  matrix& dict);
    trace get_axis(matrix& dict, int trace_size, int idx, int shift);
    matrix get_dict(int wavelet_length, T step_time_s);
    
    public:
    std::vector<std::pair<int, trace>> decompose_signal(trace gather, int trace_size, T step_time_ms, T max_amplitude);
};