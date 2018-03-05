#include "src/connector/msg_transfer/context.h"

#include "src/types/game_events.h"
#include "src/connector/msg_transfer/action.h"

connector::msg_transfer::context::context(message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl) : m_regCtrl(regCtrl), m_sessionCtrl(sessionCtrl) {
    msgCtrl.register_handler<msg_transfer::auth, types::game_events>(&m_sessionCtrl);
    msgCtrl.register_handler<msg_transfer::action, types::game_events>(&m_sessionCtrl);
}



