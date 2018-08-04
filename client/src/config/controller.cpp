#include "src/config/controller.h"

util::config_file& config::controller::local(const uint32_t id) {
    return m_localConfigs[id];
}

util::config_file& config::controller::global() {
    return m_globalConfig;
}

void config::controller::remove_local(const uint32_t id) {
    m_localConfigs.erase(id);
}

