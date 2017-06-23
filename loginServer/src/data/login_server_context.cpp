#include "include/authentication/types.h"
#include "include/util/mem.h"
#include "include/util/timestamp.h"
#include "include/data/login_server_context.h"
#include "include/encryption/randomize.h"
#include "include/message/msg_controller.h"

namespace data {
    login_server_context::login_server_context(const int maxBufferSize) {

    }

    bool login_server_context::init(database::mysql::connection& connection, int32_t serverID) {
        serverID_ = serverID;
        return loginSearch_.prepare(connection) && updateServerOfTicket_.prepare(connection) && serverSearch_.prepare(connection) && register_.prepare(connection) && unregister_.prepare(connection);
    }

    int login_server_context::getID() const {
        return serverID_;
    }

    message::msg_status_t login_server_context::authenticate(const authentication::credentials_t& credentials, authentication::ticket_t& ticket) {
        if (loginSearch_.search(credentials)) {

            ticket.accountID = loginSearch_.accountID_;
            ticket.serverID = loginSearch_.serverID_;
            ticket.permissions = loginSearch_.permissions_;
            ticket.issuer = serverID_;
            ticket.loginTime = util::timestamp::now();

            return MSG_STATUS_OK;
        }

        return MSG_STATUS_AUTH_ERROR;
    }

    message::msg_status_t login_server_context::getDestination(const authentication::ticket_t& ticket, network::ipv4_addr& addr) {
        if (serverSearch_.search(ticket.serverID)) {
            addr = serverSearch_.addr_;
            return MSG_STATUS_OK;
        }

        return MSG_STATUS_UNAVAILABLE;
    }

    message::msg_status_t login_server_context::registerService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID, const network::ipv4_addr& addr) {
        if (loginSearch_.search(credentials)) {
            // TODO add permissions

            return MSG_STATUS_EXECUTION_ERROR * !register_.addService(serverID, addr);
        }

        return MSG_STATUS_AUTH_ERROR;
    }

    message::msg_status_t login_server_context::unregisterService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID) {
        if (loginSearch_.search(credentials)) {
            // TODO add permissions

            return MSG_STATUS_EXECUTION_ERROR * !unregister_.removeService(serverID);
        }

        return MSG_STATUS_AUTH_ERROR;
    }


}
