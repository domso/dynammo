#pragma once

#include "src/game/controller.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/config/controller.h"
#include "src/encryption/private_key.h"

namespace connector {
    struct context {
        context(
            game::controller& gameCtrl,
            util::event_controller<types::game_events>& eventCtrl,
            config::controller& config
        );
        game::controller& gameCtrl;
        util::event_controller<types::game_events>& eventCtrl;
        config::controller& config;
        encryption::private_key privateKey;
    };
}
