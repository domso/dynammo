#include "src/session/user.h"


session::user::user() {
    
}

session::user::~user() {

}

bool session::user::set_accountID(const authentication::accountID_t accountID) {
    m_accountID = accountID;
    return m_publicKey.load("keys/" + std::to_string(accountID) + ".pem");
}

uint32_t session::user::get_accountID() const {
    return m_accountID;
}

void session::user::set_connection(network::tcp_connection<network::ipv4_addr>& conn) {
    m_connection = std::move(conn);
}

bool session::user::is_connected() const {
    char b;
    return m_connectionState && m_connection.recv_data(&b, 1, MSG_DONTWAIT | MSG_PEEK).first;
}

void session::user::set_addr(const network::ipv4_addr& addr) {
    m_udpAddr = addr;
}


const network::ipv4_addr& session::user::get_addr() const {
    return m_udpAddr;
}

bool session::user::verify_buffer(network::pkt_buffer& inputBuffer) {
    bool result = false;
    
    if (m_publicKey.valid()) {
        int sigLength = m_publicKey.required_size();        
        result = m_publicKey.verify<uint8_t>(inputBuffer.get_next<uint8_t>(sigLength), sigLength, inputBuffer.data(), inputBuffer.msg_length() - sigLength);
    }
    
    return result;
}

