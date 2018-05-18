#include "src/connector/context.h"

#include "src/types/game_events.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"
#include "src/connector/msg_transfer/transfer_action.h"
#include "src/connector/msg_transfer/enter_region.h"
#include "src/connector/msg_transfer/leave_region.h"

connector::msg_transfer::context::context(message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl) : m_data(regCtrl, sessionCtrl) {
    msgCtrl.register_handler<msg_transfer::auth, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::action, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::transfer_action, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::enter_region, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::leave_region, uint64_t>(&m_data);
}


