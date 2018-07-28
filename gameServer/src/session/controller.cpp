#include "src/session/controller.h"

#include "network/tcp_connection.h"
#include "network/tcp_socket.h"
#include "assert.h"
#include "src/types/data_transfer/content.h"

session::controller::controller() : m_currentSessionID(0) {

}

session::controller::~controller() {
    assert(m_running == false);
}

std::optional<util::locked_ref<session::user>> session::controller::get_user(const uint32_t sessionID) {
    std::lock_guard<std::mutex> lg(m_mutex);

    auto it = m_userInfo.find(sessionID);

    if (it != m_userInfo.end()) {
        return std::move(util::locked_ref<session::user>(it->second));
    } else {
        return std::nullopt;
    }
}

void session::controller::remove(const uint32_t sessionID) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_userInfo.erase(sessionID);
}

void session::controller::close() {
    m_running = false;
}

void session::controller::update() {
    network::tcp_socket<network::ipv4_addr> skt;    
    m_running = true;

    if (skt.accept_on(1850, 10)) {
        skt.set_timeout(1);
        while (m_running) {
            auto newConnection = skt.accept_connection();

            if (newConnection) {
                std::lock_guard<std::mutex> lg(m_mutex);
                uint32_t sessionID = next_session_id();
                m_userInfo[sessionID].set_connection(*newConnection);
                m_userInfo[sessionID].send<types::data_transfer::content::session_id>(sessionID);
            }
            
            clean();
        }
    }
}

uint32_t session::controller::next_session_id() {
    do {
        m_currentSessionID++;
    } while (m_userInfo.count(m_currentSessionID) != 0);

    return m_currentSessionID;
}

void session::controller::clean() {
    std::lock_guard<std::mutex> lg(m_mutex);
    for (auto it = m_userInfo.begin(); it != m_userInfo.end();) {
        bool result;
        {
            auto user = util::locked_ref<session::user>(it->second);
            result = user->is_connected();
        }
        
        if (result) {
            ++it;
        } else {
            it = m_userInfo.erase(it);
        }
    }
}
