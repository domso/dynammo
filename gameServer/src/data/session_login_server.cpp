#include "src/data/session_login_server.h"

#include <thread>
#include "src/data/session_context.h"
#include "src/message/msg_controller.h"
#include "src/message/msg_info.h"
#include "src/message/msg_auth.h"
#include "src/message/msg_register.h"
#include "src/message/msg_unregister.h"

namespace data {
    m_sessionlogin_server::m_sessionlogin_server(const uint16_t port, const int msgBufferSize) : m_localPort(port), m_publicKey(nullptr), m_controller(msgBufferSize), m_encryptedOutputBuffer(msgBufferSize), m_uncryptedOutputBuffer(msgBufferSize) {

    }

    m_sessionlogin_server::~m_sessionlogin_server() {
        if (m_mainThread.joinable()) {
            m_mainThread.join();
        }
    }

    bool m_sessionlogin_server::init() {
        bool result = true;
        result &= m_controller.registerHandler<message::msg_info>();
        result &= m_controller.registerHandler<message::msg_auth>();
        result &= m_controller.registerHandler<message::msg_register>();
        result &= m_controller.registerHandler<message::msg_unregister>();

        result &= m_controller.init(m_localPort, &m_context);

        if (!result) {
            return false;
        }

        m_mainThread = std::thread(&message::msg_controller<data::m_sessioncontext>::recv, &m_controller, nullptr, m_publicKey);
        return true;
    }

    void m_sessionlogin_server::close() {
        m_controller.close();
    }

    void m_sessionlogin_server::setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey) {
        m_addr = addr;
        m_publicKey = publicKey;
    }

    bool m_sessionlogin_server::requestInfo() {
        return m_controller.execRequest<message::msg_info>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool m_sessionlogin_server::waitForInfo(authentication::server_info_t& info, double timeout) {
        return m_context.waitForServerInfo(info, timeout);
    }

    bool m_sessionlogin_server::registerService(const authentication::m_serverIDt serverID, const authentication::credentials_t credentials) {
        m_context.setCredentials(credentials);
        m_context.setServerID(serverID);
        return m_controller.execRequest<message::msg_register>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool m_sessionlogin_server::unregisterService() {
        return m_controller.execRequest<message::msg_unregister>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool m_sessionlogin_server::waitForRegisterState(bool& state, double timeout) {
        return m_context.waitForRegisterState(state, timeout);
    }


    bool m_sessionlogin_server::waitForUser(authentication::user_data_t& user, double timeout) {
        return m_context.waitForUser(user, timeout);
    }

}
