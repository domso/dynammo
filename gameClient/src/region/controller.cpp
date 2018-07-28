#include "src/region/controller.h"

util::config_file& region::controller::operator[](const uint32_t regionID) {
    return m_configs[regionID];
}

void region::controller::clear_region(const uint32_t regionID) {
    m_configs.erase(regionID);
}

