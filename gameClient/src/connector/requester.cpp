#include "src/connector/requester.h"

#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/action.h"
#include "src/connector/msg_transfer/transfer_action.h"
#include "src/connector/msg_transfer/enter_region.h"
#include "src/connector/msg_transfer/leave_region.h"

connector::requester::requester(network::ipv4_addr& udpDestAddr, util::event_controller<types::game_events>& eventCtrl, message::msg_controller& msgCtrl) :
    m_buffer(bufferSize),
    m_udpDestAddr(udpDestAddr),
    m_eventCtrl(eventCtrl),
    m_msgCtrl(msgCtrl)
    {
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::recv_tcp_link, msg_transfer::auth>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_up, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_left, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_down, msg_transfer::action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_right, msg_transfer::action>>(this);
    
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_up_region, msg_transfer::transfer_action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_left_region, msg_transfer::transfer_action>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_down_region, msg_transfer::transfer_action>>(this);  
    m_eventCtrl.register_event_handler<requester::event_handler2<types::game_events::move_right_region, msg_transfer::transfer_action>>(this);  
    
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::enter_region, msg_transfer::enter_region>>(this);
    m_eventCtrl.register_event_handler<requester::event_handler<types::game_events::leave_region, msg_transfer::leave_region>>(this);
}

connector::requester::~requester() {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_eventCtrl.unregister_event_handler<requester::event_handler<types::game_events::recv_tcp_link, msg_transfer::auth>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_up, msg_transfer::action>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_left, msg_transfer::action>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_down, msg_transfer::action>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_right, msg_transfer::action>>();
        
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_up_region, msg_transfer::transfer_action>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_left_region, msg_transfer::transfer_action>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_down_region, msg_transfer::transfer_action>>();  
    m_eventCtrl.unregister_event_handler<requester::event_handler2<types::game_events::move_right_region, msg_transfer::transfer_action>>();  
    
    m_eventCtrl.unregister_event_handler<requester::event_handler<types::game_events::enter_region, msg_transfer::enter_region>>();
    m_eventCtrl.unregister_event_handler<requester::event_handler<types::game_events::leave_region, msg_transfer::leave_region>>();
}

void connector::requester::open() {
    m_open = true;
}

void connector::requester::close() {
    m_open = false;
}
