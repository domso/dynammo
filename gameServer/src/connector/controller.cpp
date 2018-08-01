#include "src/connector/controller.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"

connector::controller::controller(region::controller& rCtrl, session::controller& sCtrl) : m_context(rCtrl, sCtrl), m_msgCtrl(1500) {
    m_msgCtrl.init(1851);
    m_msgCtrl.register_handler<msg_transfer::auth>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::create_account>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::region_action>(&m_context);
    m_running = true;
}

connector::controller::~controller() {
    assert(m_running == false);
}

void connector::controller::update() {
    m_msgCtrl.recv();
}

void connector::controller::close() {
    m_msgCtrl.close();
    m_running = false;
}

