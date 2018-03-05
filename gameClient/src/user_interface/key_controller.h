#pragma once

#include <stdint.h>
#include <unordered_map>

#include "src/util/event_controller.h"
#include "src/types/game_events.h"

namespace user_interface {
    class key_controller {
    public:
        key_controller(util::event_controller<types::game_events>& eventCtrl);
        key_controller(const key_controller& copy) = delete;
        key_controller(key_controller&& move) = delete;
        ~key_controller();
        
        void key_was_pressed(uint32_t key);        
        void key_was_released(uint32_t key);
    private:        
        template <typename T>
        void register_key_event() {
            if (T::triggerOnRelease) {
                m_eventsByRelease[T::defaultKey] = T::event;
            } else {
                m_eventsByPress[T::defaultKey] = T::event;
            }
        }
        
        util::event_controller<types::game_events>& m_eventCtrl;
        std::unordered_map<uint32_t, types::game_events> m_eventsByPress;
        std::unordered_map<uint32_t, types::game_events> m_eventsByRelease;
    };
}
