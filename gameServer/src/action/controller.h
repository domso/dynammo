#pragma once

#include <type_traits>

#include "src/types/game_events.h"
#include "src/region/dynamic_obj.h"



namespace action {
    class controller {
    public:
        
        enum class return_code {
            ok,
            req_2way,
            error
        };
        
        template <typename T>        
        typename std::enable_if<std::is_same<T, region::dynamic_obj>::value, std::pair<return_code, uint32_t>>::type execute_action(T& obj, const types::game_events action) {         
            switch (action) {
                case types::game_events::move_up:    return move_up(obj);    break;
                case types::game_events::move_left:  return move_left(obj);  break;
                case types::game_events::move_down:  return move_down(obj);  break;
                case types::game_events::move_right: return move_right(obj); break;
                default: break;
            }               
            
            return {return_code::error, 0};
        }
    private:        
        std::pair<return_code, uint32_t> move_up(region::dynamic_obj& obj) {
            obj.position.x -= 0.1;
//             obj.position.y -= 1;
            obj.animation = types::game_animations::move_up;
            
            return {return_code::ok, 0};
        }
        
        std::pair<return_code, uint32_t> move_left(region::dynamic_obj& obj) {
//             obj.position.x += 1;
            obj.position.y -= 0.1;
            obj.animation = types::game_animations::move_left;
            
            return {return_code::ok, 0};
        }
        
        std::pair<return_code, uint32_t> move_down(region::dynamic_obj& obj) {
            obj.position.x += 0.1;
//             obj.position.y += 1;
            obj.animation = types::game_animations::move_down;
            
            return {return_code::ok, 0};
        }
        
        std::pair<return_code, uint32_t> move_right(region::dynamic_obj& obj) {
//             obj.position.x -= 1;
            obj.position.y += 0.1;
            obj.animation = types::game_animations::move_right;
            
            return {return_code::ok, 0};
        }
    };
}
