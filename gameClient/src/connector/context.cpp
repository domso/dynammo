#include "src/connector/context.h"

#include "src/types/game_events.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"
#include "src/connector/msg_transfer/transfer_action.h"
#include "src/connector/msg_transfer/enter_region.h"
#include "src/connector/msg_transfer/leave_region.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"

connector::msg_transfer::context::context(message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl, util::config_file& config, util::event_controller<types::game_events>& eventCtrl) : m_data(regCtrl, sessionCtrl, config, eventCtrl) {
    msgCtrl.register_handler<msg_transfer::auth, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::action, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::transfer_action, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::enter_region, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::leave_region, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::create_account, uint64_t>(&m_data);
    msgCtrl.register_handler<msg_transfer::region_action, uint64_t>(&m_data);
}


