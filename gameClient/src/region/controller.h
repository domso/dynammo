#pragma once

#include <unordered_map>
#include "src/util/config_file.h"

namespace region {
    class controller {
    public:
        util::config_file& operator[](const uint32_t regionID);
        void clear_region(const uint32_t regionID);
    private:
        std::unordered_map<uint32_t, util::config_file> m_configs;
    };
}
