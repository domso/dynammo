#include "src/connector/worker.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"
#include "src/connector/msg_transfer/transfer_action.h"
#include "src/connector/msg_transfer/enter_region.h"
#include "src/connector/msg_transfer/leave_region.h"
#include "src/connector/msg_transfer/create_account.h"

connector::worker::worker(connector::context& context) : m_context(context), m_msgCtrl(bufferSize) {

}

connector::worker::~worker() {
    m_msgCtrl.close();
    m_running = false;

    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void connector::worker::init_as(const connector::worker::types t) {
    m_running = true;

    switch (t) {
    case types::receiver:
        init_as_receiver();
        break;

    case types::updater:
        init_as_updater();
        break;

    default:
        break;
    }
}

void connector::worker::init_as_receiver() {
    m_msgCtrl.init(1851);
    m_msgCtrl.register_handler<msg_transfer::auth>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::action>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::transfer_action>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::enter_region>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::leave_region>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::create_account>(&m_context);
    
    m_thread = std::thread(&message::msg_controller::recv, &m_msgCtrl);
}

void connector::worker::init_as_updater() {
    m_thread = std::thread(&worker::update_thread_main, this);
}

void connector::worker::update_thread_main() {
    while (m_running) {
        m_context.regionCtrl.update();
    }
}


