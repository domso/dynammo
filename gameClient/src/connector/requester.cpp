#include "src/connector/requester.h"

#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"

connector::requester::requester(network::ipv4_addr& udpDestAddr, util::event_controller<types::game_events>& eventCtrl, message::msg_controller& msgCtrl, region::controller& regCtrl, session::controller& sessionCtrl) :
    m_buffer(bufferSize),
    m_udpDestAddr(udpDestAddr),
    m_eventCtrl(eventCtrl),
    m_msgCtrl(msgCtrl),
    m_regCtrl(regCtrl),
    m_sessionCtrl(sessionCtrl)
    {
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::recv_tcp_link, msg_transfer::auth>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::move_up, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::move_left, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::move_down, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::move_right, msg_transfer::action>>(this);
}



