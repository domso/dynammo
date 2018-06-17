#pragma once

#include <stdint.h>
#include "src/region/vec3.h"

namespace region {
    struct static_obj {
        vec3<uint16_t> position;
        uint8_t durability = 0;
        uint32_t type = 0;
        
        uint64_t id() const {
            return position.x + (position.y << 16) + (position.z << 16);
        }
    };
}
