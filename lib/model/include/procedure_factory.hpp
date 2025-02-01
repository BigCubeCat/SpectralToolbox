#pragma once


#include <string>
#include <vector>


/*!
 * \brief фабрика процедур!
 */
class procedure_factory {
private:
    procedure_factory() = default;

public:
    static procedure_factory *instance() {
        static procedure_factory inst;
        return &inst;
    }

    std::vector<std::string> procs();
};
