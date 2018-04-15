#include "src/session/controller.h"

session::controller::controller(util::event_controller<types::game_events>& eventCtrl, util::config_file& config) : m_eventCtrl(eventCtrl), m_config(config) {

}

int session::controller::get_signature_length() {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();
    return m_currentSession.privateKey.getRequiredSize();
}

bool session::controller::sign_data(encryption::signature& destSignature, const int8_t* data, const int length) {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();

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
    load_from_config();
    return m_currentSession.tcpTicket;
}

authentication::accountID_t session::controller::get_accountID() {
    std::lock_guard<std::mutex> lg(m_mutex);
    load_from_config();
    return m_currentSession.accountID;
}

void session::controller::load_from_config() {
    if (!m_currentSession.valid) {
        bool result = true;        
        m_currentSession.accountID = m_config.get<int>("accountID").second;        
        result &= m_currentSession.privateKey.load(m_config.get<std::string>("privateKey").second);      
        
        m_currentSession.valid = result;
    }
    
}

