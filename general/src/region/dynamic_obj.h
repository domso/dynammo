#ifndef general_region_dynamic_obj_h
#define general_region_dynamic_obj_h

#include <stdint.h>
#include "src/region/vec3.h"

namespace region {
    class dynamic_obj {
    public:
        uint32_t id;
        float speed;
        vec3<float> position;
        vec3<float> direction;
    };
}

#endif
