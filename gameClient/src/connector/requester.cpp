#include "src/connector/requester.h"

#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"

connector::requester::requester(connector::context& context, message::msg_controller& msgCtrl) :
    m_buffer(bufferSize),
    m_context(context),
    m_msgCtrl(msgCtrl) {
    m_context.eventCtrl.register_event_handler(types::game_events::enter_region, execute_callback<msg_transfer::region_action>, this);
    m_context.eventCtrl.register_event_handler(types::game_events::request_account_creation, execute_callback<msg_transfer::create_account>, this);
    m_context.eventCtrl.register_event_handler(types::game_events::recv_tcp_link, execute_callback<msg_transfer::auth>, this);

}

connector::requester::~requester() {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_context.eventCtrl.unregister_event_handler(types::game_events::request_account_creation);
    m_context.eventCtrl.unregister_event_handler(types::game_events::recv_tcp_link);
}

void connector::requester::lazy_init() {
    if (!m_open) {
        auto serverIP = m_context.config.global().get<std::string>("serverIP", "127.0.0.1");
        auto udpPort = m_context.config.global().get<uint16_t>("udpPort", 1851);
        assert(m_udpDestAddr.init(serverIP.second, udpPort.second));
        m_open = true;
    }
}
