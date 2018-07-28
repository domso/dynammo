#include "src/connector/controller.h"

#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"
#include "src/connector/msg_transfer/transfer_action.h"
#include "src/connector/msg_transfer/enter_region.h"
#include "src/connector/msg_transfer/leave_region.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"

connector::controller::controller(connector::context& context)
    : m_context(context),
      m_msgCtrl(bufferSize) {
          
    }

connector::controller::~controller() {
    m_msgCtrl.close();
}

void connector::controller::update() {
    m_msgCtrl.init(0);
    
    msgCtrl.register_handler<msg_transfer::auth, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::action, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::transfer_action, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::enter_region, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::leave_region, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::create_account, uint64_t>(&m_context);
    msgCtrl.register_handler<msg_transfer::region_action, uint64_t>(&m_context);
    
    m_msgCtrl.recv();
}

void connector::controller::close() {
    
}


