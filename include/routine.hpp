#pragma once

#include "widgets/resultdata.hpp"
#include "widgets/tracechoose.hpp"
#include "widgets/tracedata.hpp"


struct thread_arg {
    tracedata *td;
    resultdata *rd;
    trace_choose *tc;
    bool running;
};

void *routine(void *arg);
