#pragma once

#include "src/message/msg_controller.h"
#include "src/region/controller.h"
#include "src/session/controller.h"
#include "src/connector/data.h"

namespace connector {
    namespace msg_transfer {
        class context {
        public:
            context(message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl, util::config_file& config, util::event_controller<types::game_events>& eventCtrl);
            
        private:
            data m_data; 
        };
    }
}
