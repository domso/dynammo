#include <cstring>
#include "src/authentication/types.h"
#include "src/encryption/randomize.h"
#include "src/util/timestamp.h"
#include "src/util/mem.h"
#include "src/data/login_context.h"

namespace data {

    encryption::public_key& login_context::getKey() {
        return m_key;
    }

    void login_context::getServerInfo(authentication::server_info_t& info) {
        util::wait_lock::read<authentication::server_info_t> lock(m_serverInfo);
        info = lock.data();
    }

    void login_context::setServerInfo(const authentication::server_info_t& info) {
        util::wait_lock::write<authentication::server_info_t> lock(m_serverInfo);
        lock.data() = info;
    }

    bool login_context::waitForServerInfo(authentication::server_info_t& info, double timeOut) {
        util::wait_lock::wait<authentication::server_info_t> lock(m_serverInfo, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        info = lock.data();
        return true;
    }

    void login_context::getCredentials(authentication::credentials_t& credentials) {
        util::wait_lock::read<authentication::credentials_t> lock(m_credentials);
        credentials = lock.data();
    }

    void login_context::setCredentials(const authentication::credentials_t& credentials) {
        util::wait_lock::write<authentication::credentials_t> lock(m_credentials);
        lock.data() = credentials;
    }

    void login_context::getSession(authentication::session_t& session) {
        util::wait_lock::read<authentication::session_t> lock(m_session);
        session = lock.data();
    }

    void login_context::setSession(const authentication::session_t& session) {
        util::wait_lock::write<authentication::session_t> lock(m_session);
        lock.data() = session;
    }

    bool login_context::waitForSession(authentication::session_t& session, double timeOut) {
        util::wait_lock::wait<authentication::session_t> lock(m_session, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        session = lock.data();
        return true;
    }

}
