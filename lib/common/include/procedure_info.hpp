#pragma once

#include <string>
#include <vector>

#include "description.hpp"


/*!
 * \brief информация о настройках процедуры
 */
class procedure_info {
private:
    std::string m_name;

public:
    explicit procedure_info(std::string name);

    virtual std::vector<param_description> description() = 0;
};
