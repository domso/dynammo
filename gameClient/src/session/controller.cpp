#include "src/session/controller.h"

#include <iostream>
session::controller::controller(util::event_controller<types::game_events>& eventCtrl) : m_eventCtrl(eventCtrl) {
    load_local_account(0);
}

void session::controller::load_local_account(const authentication::accountID_t accID) {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    m_currentSession.accountID = accID;
    m_currentSession.valid = m_currentSession.privateKey.load("../../keys/private.pem");
}

void session::controller::close_local_account(const authentication::accountID_t accID) {
    std::lock_guard<std::mutex> lg(m_mutex);
}

int session::controller::get_signature_length() {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_currentSession.privateKey.getRequiredSize();
}

bool session::controller::sign_data(encryption::signature& destSignature, const int8_t* data, const int length) {
    std::lock_guard<std::mutex> lg(m_mutex);

    if (m_currentSession.valid) {          
        return encryption::sign<int8_t>(m_currentSession.privateKey, destSignature, data, length) > 0;
    } else {
        return false;
    }    
}

void session::controller::set_tcp_link(const authentication::ticket_t newLink) {
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_currentSession.tcpTicket = newLink;
    }
    // no mutex shall be locked while creating an event!
    m_eventCtrl.new_event(types::game_events::recv_tcp_link);
}

authentication::ticket_t session::controller::get_tcp_link() {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_currentSession.tcpTicket;
}

