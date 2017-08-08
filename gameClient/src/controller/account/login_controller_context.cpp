#include <cstring>
#include <vector>
#include "src/authentication/types.h"
#include "src/encryption/randomize.h"
#include "src/util/timestamp.h"
#include "src/util/mem.h"
#include "src/controller/account/login_controller_context.h"

namespace controller {

    encryption::public_key& login_controller_context::getKey() {
        return m_key;
    }

    void login_controller_context::getServerInfo(authentication::server_info_t& info) {
        util::wait_lock::read<authentication::server_info_t> lock(m_serverInfo);
        info = lock.data();
    }

    void login_controller_context::setServerInfo(const authentication::server_info_t& info) {
        util::wait_lock::write<authentication::server_info_t> lock(m_serverInfo);
        lock.data() = info;
    }

    bool login_controller_context::waitForServerInfo(authentication::server_info_t& info, double timeOut) {
        util::wait_lock::wait<authentication::server_info_t> lock(m_serverInfo, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        info = lock.data();
        return true;
    }

    void login_controller_context::getCredentials(authentication::credentials_t& credentials) {
        util::wait_lock::read<authentication::credentials_t> lock(m_credentials);
        credentials = lock.data();
    }

    void login_controller_context::setCredentials(const authentication::credentials_t& credentials) {
        util::wait_lock::write<authentication::credentials_t> lock(m_credentials);
        lock.data() = credentials;
    }

    void login_controller_context::getSession(authentication::signed_session_t& session) {
        util::wait_lock::read<authentication::signed_session_t> lock(m_signedSession);
        session = lock.data();
    }

    void login_controller_context::setSession(const authentication::session_t& session, const int sigLen, const unsigned char* signature) {
        util::wait_lock::write<authentication::signed_session_t> lock(m_signedSession);
        lock.data().session = session;
        lock.data().signature = std::vector<unsigned char>(signature, signature + sigLen);       
    }

    bool login_controller_context::waitForSession(authentication::signed_session_t& session, double timeOut) {
        util::wait_lock::wait<authentication::signed_session_t> lock(m_signedSession, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        session = lock.data();
        return true;
    }

    void login_controller_context::getLoginState(bool& loginState) {
        util::wait_lock::read<bool> lock(m_loginState);
        loginState = lock.data();
    }

    void login_controller_context::setLoginState(const bool& loginState) {
        util::wait_lock::write<bool> lock(m_loginState);
        lock.data() = loginState;
    }

    bool login_controller_context::waitForLoginState(bool& loginState, double timeOut) {
        util::wait_lock::wait<bool> lock(m_loginState, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        loginState = lock.data();
        return true;
    }
}
