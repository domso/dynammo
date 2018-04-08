#pragma once

#include <stdint.h>
#include "src/region/vec3.h"

namespace region {
    struct static_obj {
        vec3<uint8_t> position;
        uint8_t durability;
        uint32_t type;
        
        uint32_t id() const {
            return position.x + (position.y << 8) + (position.z << 16);
        }
    };
}
