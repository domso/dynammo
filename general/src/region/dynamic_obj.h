#ifndef general_region_dynamic_obj_h
#define general_region_dynamic_obj_h

#include <stdint.h>
#include "src/region/vec3.h"
#include "src/types/game_animations.h"

namespace region {
    struct dynamic_obj {
        uint32_t id;
        uint8_t health;
        
        float speed;
        vec3<float> position;
        vec3<float> direction;
        types::game_animations::values animation;
    };
}

#endif
