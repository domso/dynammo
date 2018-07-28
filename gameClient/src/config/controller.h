#pragma once

#include <unordered_map>
#include "src/util/config_file.h"

namespace config {
    class controller {
    public:
        util::config_file& local(const uint32_t id);
        util::config_file& global();
        
        void remove_local(const uint32_t id);
    private:
        util::config_file m_globalConfig;
        std::unordered_map<uint32_t, util::config_file> m_localConfigs;
    };
}
