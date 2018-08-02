#include "src/connector/controller.h"

#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"
#include "src/connector/msg_transfer/region_chat.h"

connector::controller::controller(connector::context& context)
    : m_context(context),
      m_msgCtrl(bufferSize),
      m_requester(context, m_msgCtrl)
      {
          
    }

connector::controller::~controller() {
    m_msgCtrl.close();
}

void connector::controller::update() {
    m_msgCtrl.init(0);
    
    m_msgCtrl.register_handler<msg_transfer::auth, uint64_t>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::create_account, uint64_t>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::region_action, uint64_t>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::region_chat, uint64_t>(&m_context);
    
    m_msgCtrl.recv();
}

void connector::controller::close() {
     m_msgCtrl.close();
}


