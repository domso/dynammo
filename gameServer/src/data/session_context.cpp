#include <cstring>
#include "include/authentication/types.h"
#include "include/encryption/randomize.h"
#include "include/util/timestamp.h"
#include "include/util/mem.h"
#include "include/data/session_context.h"

namespace data {

    encryption::public_key& session_context::getKey() {
        return key_;
    }

    void session_context::getServerInfo(authentication::server_info_t& info) {
        util::wait_lock::read<authentication::server_info_t> lock(serverInfo_);
        info = lock.data();
    }

    void session_context::setServerInfo(const authentication::server_info_t& info) {
        util::wait_lock::write<authentication::server_info_t> lock(serverInfo_);
        lock.data() = info;
    }

    bool session_context::waitForServerInfo(authentication::server_info_t& info, double timeOut) {
        util::wait_lock::wait<authentication::server_info_t> lock(serverInfo_, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        info = lock.data();
        return true;
    }

    void session_context::getCredentials(authentication::credentials_t& credentials) {
        util::wait_lock::read<authentication::credentials_t> lock(credentials_);
        credentials = lock.data();
    }

    void session_context::setCredentials(const authentication::credentials_t& credentials) {
        util::wait_lock::write<authentication::credentials_t> lock(credentials_);
        lock.data() = credentials;
    }

    void session_context::createNewUser(const authentication::ticket_t& ticket, authentication::session_t& session) {
        util::wait_lock::write<std::queue<authentication::user_data_t>> lock(users_);
        lock.data().emplace<>();
        authentication::user_data_t& user = lock.data().back();

        session.accountID = ticket.accountID;
        encryption::randomize<authentication::sessionID_t>(&session.sessionID);

        user.ticket = ticket;
        user.session = session;

    }

    bool session_context::waitForUser(authentication::user_data_t& user, double timeOut) {
        util::wait_lock::wait<std::queue<authentication::user_data_t>> lock(users_, timeOut);

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

    void session_context::getServerID(authentication::serverID_t& serverID) {
        util::wait_lock::read<authentication::serverID_t> lock(serverID_);
        serverID = lock.data();
    }

    void session_context::setServerID(const authentication::serverID_t& serverID) {
        util::wait_lock::write<authentication::serverID_t> lock(serverID_);
        lock.data() = serverID;
    }

    void session_context::setRegisterState(const bool state) {
        util::wait_lock::write<bool> lock(registerState_);
        lock.data() = state;
    }

    bool session_context::waitForRegisterState(bool& state, double timeOut) {
        util::wait_lock::wait<bool> lock(registerState_, timeOut);

        if (!lock.isValid()) {
            return false;
        }
        state = lock.data();
        return true;
    }

}
