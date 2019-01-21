#pragma once

#include "src/types/game_events.h"

namespace user_interface {
    namespace key_bindings {
        struct move_up {
            constexpr static const auto title            = "move_up";
            constexpr static const auto defaultKey       = 'w';
            constexpr static const auto event            = types::game_events::move_up;
            constexpr static const auto triggerOnRelease = false;
        };        
        
        struct move_left {
            constexpr static const auto title            = "move_left";
            constexpr static const auto defaultKey       = 'a';
            constexpr static const auto event            = types::game_events::move_left;
            constexpr static const auto triggerOnRelease = false;
        };
        
        struct move_down {
            constexpr static const auto title            = "move_down";
            constexpr static const auto defaultKey       = 's';
            constexpr static const auto event            = types::game_events::move_down;
            constexpr static const auto triggerOnRelease = false;
        };
        
        struct move_right {
            constexpr static const auto title            = "move_right";
            constexpr static const auto defaultKey       = 'd';
            constexpr static const auto event            = types::game_events::move_right;
            constexpr static const auto triggerOnRelease = false;
        };
        
        struct attack {
            constexpr static const auto title            = "attack";
            constexpr static const auto defaultKey       = ' ';
            constexpr static const auto event            = types::game_events::attack;
            constexpr static const auto triggerOnRelease = false;
        };
    }
}
