#pragma once

#include <unordered_map>

#include "src/user_interface/controller.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/config/controller.h"

#include "src/graphic/renderer.h"
#include "src/game/mesh_factory.h"
#include "src/game/context.h"

#include <mutex>

namespace game {
    class controller {
    public:
        controller(user_interface::controller& interfaceCtrl, util::event_controller<types::game_events>& eventCtrl, config::controller& config);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();
        
        template <typename T>
        void add_game_object(const uint32_t regionID, T& input) {
            std::lock_guard<std::mutex> lg(m_mutex);
            auto& region = m_gameObjects.try_emplace(regionID, m_meshFactory).first->second;            
            region.add_game_object(input);            
        }
    private:    
        std::mutex m_mutex;
        graphic::renderer& m_renderer;        
        mesh_factory m_meshFactory;
        util::event_controller<types::game_events>& m_eventCtrl;
        
        std::unordered_map<uint32_t, game::context> m_gameObjects;
        
        config::controller& m_config;
    };
}
