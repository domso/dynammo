#include "src/session/controller.h"

session::controller::controller(util::event_controller<types::game_events>& eventCtrl, util::config_file& config) : m_eventCtrl(eventCtrl), m_config(config) {

}

int session::controller::get_signature_length() {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();
    return m_currentSession.privateKey.required_size();
}

bool session::controller::sign_data(uint8_t* signature, const int sigLength, const int8_t* data, const int length) {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();

    if (m_currentSession.valid) {
        return m_currentSession.privateKey.sign<int8_t>(signature, sigLength, data, length) > 0;
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
//     m_eventCtrl.new_event(types::game_events::recv_tcp_link);
}

authentication::ticket_t session::controller::get_tcp_link() {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();
    return m_currentSession.tcpTicket;
}

authentication::accountID_t session::controller::get_accountID() {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();
    return m_currentSession.accountID;
}

void session::controller::set_auth_state(const bool state) {
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_currentSession.auth_state = state;
    }
    
    if (state) {        
//         m_eventCtrl.new_event(types::game_events::enter_region, 0xFFFF'FFFF);
//         m_eventCtrl.new_event(types::game_events::enter_region, 0x0000'FFFF);
//         m_eventCtrl.new_event(types::game_events::enter_region, 0x0001'FFFF);
//         
//         m_eventCtrl.new_event(types::game_events::enter_region, 0xFFFF'0000);
        m_eventCtrl.new_event(types::game_events::enter_region, 0x0000'0000);
//         m_eventCtrl.new_event(types::game_events::enter_region, 0x0001'0000);
//         
//         m_eventCtrl.new_event(types::game_events::enter_region, 0xFFFF'0001);
//         m_eventCtrl.new_event(types::game_events::enter_region, 0x0000'0001);
//         m_eventCtrl.new_event(types::game_events::enter_region, 0x0001'0001);
    }
}

bool session::controller::get_auth_state() {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_currentSession.auth_state;
}


void session::controller::load_from_config() {
    if (!m_currentSession.valid) {
        bool result = true;
        m_currentSession.accountID = m_config.get<int>("accountID", 1).second;
        result &= m_currentSession.privateKey.load(m_config.get<std::string>("privateKey").second);

        m_currentSession.valid = result;
    }

}

