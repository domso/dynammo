#include "src/authentication/types.h"
#include "src/util/mem.h"
#include "src/util/timestamp.h"
#include "src/data/login_server_context.h"
#include "src/encryption/randomize.h"
#include "src/message/msg_controller.h"

namespace data {
    login_server_context::login_server_context(const int maxBufferSize) {

    }

    bool login_server_context::init(database::mysql::connection& connection, int32_t serverID) {
        m_serverID = serverID;
        return m_loginSearch.prepare(connection) && m_updateServerOfTicket.prepare(connection) && m_serverSearch.prepare(connection) && m_register.prepare(connection) && m_unregister.prepare(connection);
    }

    int login_server_context::getID() const {
        return m_serverID;
    }

    message::msg_status_t login_server_context::authenticate(const authentication::credentials_t& credentials, authentication::ticket_t& ticket) {
        if (m_loginSearch.search(credentials)) {

            ticket.accountID = m_loginSearch.m_accountID;
            ticket.serverID = m_loginSearch.m_serverID;
            ticket.permissions = m_loginSearch.m_permissions;
            ticket.issuer = m_serverID;
            ticket.loginTime = util::timestamp::now();

            return MSG_STATUS_OK;
        }

        return MSG_STATUS_AUTH_ERROR;
    }

    message::msg_status_t login_server_context::getDestination(const authentication::ticket_t& ticket, network::ipv4_addr& addr) {
        if (m_serverSearch.search(ticket.serverID)) {
            addr = m_serverSearch.m_addr;
            return MSG_STATUS_OK;
        }

        return MSG_STATUS_UNAVAILABLE;
    }

    message::msg_status_t login_server_context::registerService(const authentication::credentials_t& credentials, const authentication::m_serverIDt& serverID, const network::ipv4_addr& addr) {
        if (m_loginSearch.search(credentials)) {
            // TODO add permissions

            return MSG_STATUS_EXECUTION_ERROR * !m_register.addService(serverID, addr);
        }

        return MSG_STATUS_AUTH_ERROR;
    }

    message::msg_status_t login_server_context::unregisterService(const authentication::credentials_t& credentials, const authentication::m_serverIDt& serverID) {
        if (m_loginSearch.search(credentials)) {
            // TODO add permissions

            return MSG_STATUS_EXECUTION_ERROR * !m_unregister.removeService(serverID);
        }

        return MSG_STATUS_AUTH_ERROR;
    }


}
