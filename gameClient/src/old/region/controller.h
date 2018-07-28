#ifndef gameClient_region_controller_h
#define gameClient_region_controller_h

#include <unordered_map>
#include <queue>
#include <mutex>
#include "src/util/lock_ref.h"
#include "src/region/context.h"
#include "src/session/controller.h"

namespace region {
    class controller {
    public:
        controller(graphic::controller& graphicCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        
        util::locked_ref<region::context> operator[](const uint32_t id);
        void remove(const uint32_t id);
        
        void select_region(const uint32_t id);
        util::locked_ref<region::context> get_selected_region();
    private:
        util::locked_ref<region::context> try_insert(const uint32_t id);       
        
        
        template <types::game_events triggerT>
        struct event_handler {
            constexpr static const auto trigger = triggerT;
            static types::game_events handle(const types::game_events event, const uint64_t& arg, region::controller* ctrl) {
//                 std::lock_guard<std::mutex> lg(ctrl->m_mutex);
                auto region = (*ctrl)[0];
                int id = region->get_selected_character();
                auto obj = region->get_dynamic_obj(id);
                
                if (obj.position.x < 0) {
                    return types::game_events::move_left_region;
                }
                
                if (obj.position.y < 0) {
                    return types::game_events::move_up_region;
                }
                
                if (obj.position.x > 128) {
                    return types::game_events::move_right_region;
                }
                
                if (obj.position.x > 128) {
                    return types::game_events::move_down_region;
                }
                
                if (event == types::game_events::key_up) {
                    return types::game_events::move_up;
                }
                
                if (event == types::game_events::key_down) {
                    return types::game_events::move_down;
                }
                
                if (event == types::game_events::key_right) {
                    return types::game_events::move_right;
                }
                
                if (event == types::game_events::key_left) {
                    return types::game_events::move_left;
                }
                
//                 ctrl->m_sessionCtrl.get
                
                return types::game_events::clear;
            }
        };
        
        
        uint32_t m_selectedRegion;
        
        graphic::controller& m_graphicCtrl;
        session::controller& m_sessionCtrl;
        util::event_controller<types::game_events>& m_eventCtrl;
        std::unordered_map<uint32_t, region::context> m_regionData;
        std::mutex m_mutex;
    };
}

#endif
