#include "src/controller/account/login_controller.h"

#include <thread>
#include "src/controller/account/login_controller_context.h"
#include "src/message/msg_controller.h"
#include "src/message/account/msg_info.h"
#include "src/message/account/msg_auth.h"

namespace controller {
    login_controller::login_controller(const uint16_t port, const int msgBufferSize) : m_localPort(port), m_publicKey(nullptr), m_controller(msgBufferSize), m_encryptedOutputBuffer(msgBufferSize), m_uncryptedOutputBuffer(msgBufferSize) {

    }

    login_controller::~login_controller() {
        if (m_mainThread.joinable()) {
            m_mainThread.join();
        }
    }

    bool login_controller::init() {
        bool result = true;
        result &= m_controller.registerHandler<message::account::msg_info>();
        result &= m_controller.registerHandler<message::account::msg_auth>();

        result &= m_controller.init(m_localPort, &m_context);

        if (!result) {
            return false;
        }

        m_mainThread = std::thread(&message::msg_controller<controller::login_controller_context>::recv, &m_controller, nullptr, m_publicKey);
        return true;
    }

    void login_controller::close() {
        m_controller.close();
    }

    void login_controller::setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey) {
        m_addr = addr;
        m_publicKey = publicKey;
    }

    bool login_controller::requestInfo() {
        return m_controller.execRequest<message::account::msg_info>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool login_controller::waitForInfo(authentication::server_info_t& info, double timeout) {
        return m_context.waitForServerInfo(info, timeout);
    }

    bool login_controller::requestSession(const authentication::credentials_t& credentials) {
        m_context.setCredentials(credentials);
        return m_controller.execRequest<message::account::msg_auth>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool login_controller::waitForSession(authentication::signed_session_t& session, double timeout) {
        return m_context.waitForSession(session, timeout);
    }
    
    bool login_controller::requestLogin() {
        return m_controller.execRequest<message::account::msg_auth>(m_addr, m_uncryptedOutputBuffer, m_encryptedOutputBuffer, m_context, nullptr, m_publicKey);
    }

    bool login_controller::waitForLogin(bool& loginState, double timeout) {
        return m_context.waitForLoginState(loginState, timeout);
    }
}
