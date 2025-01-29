#include "datamodel.hpp"


proc_config_iface *datamodel::get_params() {
    return m_config.get();
}
