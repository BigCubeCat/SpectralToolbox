#pragma once

#include "widgets/resultdata.hpp"
#include "widgets/tracedata.hpp"


struct routine_arg {
    tracedata *td;
    resultdata *rd;
    bool running;
};

void *routine(void *arg);
