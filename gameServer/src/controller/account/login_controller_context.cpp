#include <cstring>
#include "src/authentication/types.h"
#include "src/encryption/randomize.h"
#include "src/util/timestamp.h"
#include "src/util/mem.h"
#include "src/controller/account/login_controller_context.h"

namespace controller {
    namespace account {
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

        void login_controller_context::getServerID(authentication::serverID_t& serverID) {
            util::wait_lock::read<authentication::serverID_t> lock(m_serverID);
            serverID = lock.data();
        }

        void login_controller_context::setServerID(const authentication::serverID_t& serverID) {
            util::wait_lock::write<authentication::serverID_t> lock(m_serverID);
            lock.data() = serverID;
        }

        void login_controller_context::setRegisterState(const bool state) {
            util::wait_lock::write<bool> lock(m_registerState);
            lock.data() = state;
        }

        bool login_controller_context::waitForRegisterState(bool& state, double timeOut) {
            util::wait_lock::wait<bool> lock(m_registerState, timeOut);

            if (!lock.isValid()) {
                return false;
            }

            state = lock.data();
            return true;
        }
    }
}
