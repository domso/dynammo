#include "src/connector/worker.h"
#include "src/connector/message/auth.h"
#include "src/connector/message/action.h"

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
    m_msgCtrl.register_handler<message::callback::auth>(&m_context);
    m_msgCtrl.register_handler<message::callback::action>(&m_context);
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


