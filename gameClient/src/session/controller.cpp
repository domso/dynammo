#include "src/session/controller.h"

session::controller::controller(util::event_controller<types::game_events>& eventCtrl) : m_eventCtrl(eventCtrl) {
}

void session::controller::set_tcp_link(authentication::ticket_t newLink) {
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_tcpTicket = newLink;
    }
    // no mutex shall be locked while creating an event!
    m_eventCtrl.new_event(types::game_events::recv_tcp_link);     
}

authentication::ticket_t session::controller::get_tcp_link() {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_tcpTicket;
}

