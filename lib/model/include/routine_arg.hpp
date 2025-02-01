#pragma once


#include <string>


/*!
 * \brief аргумент для потока-вычислителя
 */
struct routine_arg {
    std::string input_file;
    std::string output_dir;

    std::string red_file;
    std::string green_file;
    std::string blue_file;

    float time = 0.0001f;
    float amp  = 0.0001f;

    int red;
    int green;
    int blue;
};
