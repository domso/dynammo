#pragma once

#include "src/message/msg_controller.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    namespace msg_transfer {
        class context {
        public:
            context(message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl);
        private:
            region::controller& m_regCtrl;
            session::controller& m_sessionCtrl;
        };
    }
}
