#include "src/authentication/types.h"
#include "src/util/mem.h"
#include "src/util/timestamp.h"
#include "src/controller/login_server_context.h"
#include "src/encryption/randomize.h"
#include "src/message/msg_controller.h"

namespace controller {
    login_server_context::login_server_context(const int maxBufferSize) {

    }

    bool login_server_context::init(database::mysql::connection& connection, int32_t serverID, encryption::private_key* privateKey) {
        m_serverID = serverID;
        m_privateKey = privateKey;
        return m_loginSearch.prepare(connection) && m_updateServerOfTicket.prepare(connection) && m_serverSearch.prepare(connection) && m_register.prepare(connection) && m_unregister.prepare(connection);
    }

    int login_server_context::getID() const {
        return m_serverID;
    }

    message::msg_status_t login_server_context::authenticate(const authentication::credentials_t& credentials, authentication::session_t& session) {
        if (m_loginSearch.search(credentials)) {
            
            encryption::randomize<authentication::sessionID_t>(&session.sessionID);
            session.accountID = m_loginSearch.m_accountID;
            session.serverID = m_loginSearch.m_serverID;
            session.permissions = m_loginSearch.m_permissions;
            session.issuer = m_serverID;
            session.loginTime = util::timestamp::now();

            return message::status::ok;
        }

        return message::status::error::auth;
    }

    message::msg_status_t login_server_context::getDestination(const authentication::session_t& session, network::ipv4_addr& addr) {
        if (m_serverSearch.search(session.serverID)) {
            addr = m_serverSearch.m_addr;
            return message::status::ok;
        }

        return message::status::error::unavailable;
    }

    message::msg_status_t login_server_context::registerService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID, const network::ipv4_addr& addr) {
        if (m_loginSearch.search(credentials)) {
            // TODO add permissions

            return message::status::error::execution * !m_register.addService(serverID, addr);
        }

        return message::status::error::auth;
    }

    message::msg_status_t login_server_context::unregisterService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID) {
        if (m_loginSearch.search(credentials)) {
            // TODO add permissions

            return message::status::error::execution * !m_unregister.removeService(serverID);
        }

        return message::status::error::auth;
    }


}
