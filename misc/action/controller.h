#pragma once

#include <type_traits>

#include "src/types/game_events.h"
#include "src/region/dynamic_obj.h"
#include "src/region/controller.h"
#include <optional>


namespace action {
    class controller {
    public:

        controller(region::controller& regionCtrl);

        enum class return_code {
            ok,
            req_2way,
            error
        };

        std::optional<region::dynamic_obj> execute_action(const uint32_t regionID, const uint32_t objID, const types::game_events action);

    private:
//         std::pair<return_code, uint32_t> move_up(region::dynamic_obj& obj, region::context& region) {
//             auto& layer = region.get_obj_layer(obj.position.z);
// 
//             if (layer.get_nearest(obj.position.x - 1, obj.position.y).durability == 0) {
//                 obj.position.x -= 1;
//             }            
//             obj.animation = types::game_animations::move_up;
// 
//             return {return_code::ok, 0};
//         }
// 
//         std::pair<return_code, uint32_t> move_left(region::dynamic_obj& obj, region::context& region) {
//             auto& layer = region.get_obj_layer(obj.position.z);
// 
//             if (layer.get_nearest(obj.position.x, obj.position.y - 1).durability == 0) {
//                 obj.position.y -= 1;
//             }
//             obj.animation = types::game_animations::move_left;
// 
//             return {return_code::ok, 0};
//         }
// 
//         std::pair<return_code, uint32_t> move_down(region::dynamic_obj& obj, region::context& region) {
//             auto& layer = region.get_obj_layer(obj.position.z);
// 
//             if (layer.get_nearest(obj.position.x + 1, obj.position.y).durability == 0) {
//                 obj.position.x += 1;
//             }
//             obj.animation = types::game_animations::move_down;
// 
//             return {return_code::ok, 0};
//         }
// 
//         std::pair<return_code, uint32_t> move_right(region::dynamic_obj& obj, region::context& region) {
//             auto& layer = region.get_obj_layer(obj.position.z);
// 
//             if (layer.get_nearest(obj.position.x, obj.position.y + 1).durability == 0) {
//                 obj.position.y += 1;
//             }
//             obj.animation = types::game_animations::move_right;
// 
//             return {return_code::ok, 0};
//         }


        region::controller& m_regionCtrl;
    };
}
