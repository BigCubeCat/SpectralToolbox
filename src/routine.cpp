#include "../include/routine.hpp"

#include "datamodel.hpp"

void *routine(void *arg) {
    auto *argument = static_cast<routine_arg *>(arg);
    auto *model    = datamodel::instance();

    while (argument->running) {
        if (argument->td->need_update()) {
            argument->td->update_image();
        }
        if (argument->rd->need_update() || model->calculation_is_done.load()) {
            argument->rd->update_image();
        }
        if (argument->tc->need_update()) {
            argument->tc->refresh();
        }
    }
    return nullptr;
}
