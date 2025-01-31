#include "spectrum_decomposer_i.hpp"
#include "matching_pursuit.h"


class mp_decomposer: public spectrum_decomposer_i {

public:
    mp_decomposer() = default;
    mp_decomposer(float step_time_s, float max_amplitude);
    std::vector<float_trace> examine(const std::string &path, long index) override;

    void decompose(
        const std::string &input_file,
        const std::string &output_dir,
        int red,
        int green,
        int blue
    ) override;

private:
    float m_step_time_s;
    float m_max_amplitude;
    matching_pursuit<float> decomposer;
};

