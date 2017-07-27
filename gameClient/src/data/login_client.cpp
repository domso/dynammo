#include "src/data/login_client.h"

#include <thread>
#include "src/data/login_context.h"
#include "src/message/msg_controller.h"
#include "src/message/msg_info.h"
#include "src/message/msg_auth.h"

namespace data {
    login_client::login_client(const uint16_t port, const int msgBufferSize) : m_localPort(port), m_publicKey(nullptr), m_controller(msgBufferSize), m_encryptedOutputBuffer(msgBufferSize), m_uncryptedOutputBuffer(msgBufferSize) {

    }

    login_client::~login_client() {
        if (m_mainThread.joinable()) {
            m_mainThread.join();
        }
    }

    bool login_client::init() {
        bool result = true;
        result &= m_controller.registerHandler<message::msg_info>();
        result &= m_controller.registerHandler<message::msg_auth>();

        result &= m_controller.init(m_localPort, &m_context);

        if (!result) {
            return false;
        }

        m_mainThread = std::thread(&message::msg_controller<data::login_context>::recv, &m_controller, nullptr, m_publicKey);
        return true;
    }

    void login_client::close() {
        m_controller.close();
    }

    void login_client::setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey) {
        m_addr = addr;
        m_publicKey = publicKey;
    }

    bool login_client::requestInfo() {
        return m_controller.execRequest<message::msg_info>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool login_client::waitForInfo(authentication::server_info_t& info, double timeout) {
        return m_context.waitForServerInfo(info, timeout);
    }

    bool login_client::requestSession(const authentication::credentials_t& credentials) {
        m_context.setCredentials(credentials);
        return m_controller.execRequest<message::msg_auth>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool login_client::waitForSession(authentication::session_t& session, double timeout) {
        return m_context.waitForSession(session, timeout);
    }
}
