#pragma once

#include "src/region/controller.h"
#include "src/session/controller.h"
#include "src/util/config_file.h"

namespace connector {
    namespace msg_transfer {
        struct data {
            data(region::controller& regCtrl, session::controller& sessionCtrl, util::config_file& config, util::event_controller<types::game_events>& eventCtrl);
            region::controller& regionCtrl;
            session::controller& sessionCtrl;
            util::config_file& config;
            util::event_controller<types::game_events>& eventCtrl;
        };
    }
}
