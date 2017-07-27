#include <cstring>
#include "src/authentication/types.h"
#include "src/encryption/randomize.h"
#include "src/util/timestamp.h"
#include "src/util/mem.h"
#include "src/data/session_context.h"

namespace data {

    encryption::public_key& session_context::getKey() {
        return m_key;
    }

    void session_context::getServerInfo(authentication::server_info_t& info) {
        util::wait_lock::read<authentication::server_info_t> lock(m_serverInfo);
        info = lock.data();
    }

    void session_context::setServerInfo(const authentication::server_info_t& info) {
        util::wait_lock::write<authentication::server_info_t> lock(m_serverInfo);
        lock.data() = info;
    }

    bool session_context::waitForServerInfo(authentication::server_info_t& info, double timeOut) {
        util::wait_lock::wait<authentication::server_info_t> lock(m_serverInfo, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        info = lock.data();
        return true;
    }

    void session_context::getCredentials(authentication::credentials_t& credentials) {
        util::wait_lock::read<authentication::credentials_t> lock(m_credentials);
        credentials = lock.data();
    }

    void session_context::setCredentials(const authentication::credentials_t& credentials) {
        util::wait_lock::write<authentication::credentials_t> lock(m_credentials);
        lock.data() = credentials;
    }

    void session_context::createNewUser(const authentication::ticket_t& ticket, authentication::session_t& session) {
        util::wait_lock::write<std::queue<authentication::user_data_t>> lock(m_users);
        lock.data().emplace<>();
        authentication::user_data_t& user = lock.data().back();

        session.accountID = ticket.accountID;
        encryption::randomize<authentication::sessionID_t>(&session.sessionID);

        user.ticket = ticket;
        user.session = session;

    }

    bool session_context::waitForUser(authentication::user_data_t& user, double timeOut) {
        util::wait_lock::wait<std::queue<authentication::user_data_t>> lock(m_users, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        if (lock.data().size() > 0) {
            user = lock.data().front();
            lock.data().pop();
            return true;
        }

        return false;
    }

    void session_context::getServerID(authentication::m_serverIDt& serverID) {
        util::wait_lock::read<authentication::m_serverIDt> lock(m_serverID);
        serverID = lock.data();
    }

    void session_context::setServerID(const authentication::m_serverIDt& serverID) {
        util::wait_lock::write<authentication::m_serverIDt> lock(m_serverID);
        lock.data() = serverID;
    }

    void session_context::setRegisterState(const bool state) {
        util::wait_lock::write<bool> lock(m_registerState);
        lock.data() = state;
    }

    bool session_context::waitForRegisterState(bool& state, double timeOut) {
        util::wait_lock::wait<bool> lock(m_registerState, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        state = lock.data();
        return true;
    }

}
