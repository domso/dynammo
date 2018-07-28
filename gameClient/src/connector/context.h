#pragma once


#include "src/util/config_file.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/region/controller.h"
#include "src/graphic/controller.h"

namespace connector {
    struct context {
        context(
            region::controller& regCtrl,
            graphic::controller& graphicCtrl,
            util::config_file& config,
            util::event_controller<types::game_events>& eventCtrl
        );
        region::controller& regionCtrl;
        graphic::controller& graphicCtrl;
        util::config_file& config;
        util::event_controller<types::game_events>& eventCtrl;
    };
}
