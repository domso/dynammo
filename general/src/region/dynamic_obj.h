#ifndef general_region_dynamic_obj_h
#define general_region_dynamic_obj_h

#include <stdint.h>
#include "src/region/vec3.h"
#include "src/types/game_events.h"

#include <iostream>

namespace region {
    class dynamic_obj {
    public:
        uint32_t id;
        float speed;
        vec3<float> position;
        vec3<float> direction;
        
        bool action(const types::game_events action) {
            switch (action) {
                case types::game_events::move_up:    move_up();    break;
                case types::game_events::move_left:  move_left();  break;
                case types::game_events::move_down:  move_down();  break;
                case types::game_events::move_right: move_right(); break;
                default: break;
            }
            
            return true;
        }
        
    private:
        void move_up() {
            position.x -= 1;
            position.y -= 1;
        }
        void move_left() {
            position.x += 1;
            position.y -= 1;
        }
        void move_down() {
            position.x += 1;
            position.y += 1;
        }
        void move_right() {
            position.x -= 1;
            position.y += 1;
        }
    };
}

#endif
