#include "include/authentication/types.h"
#include "include/util/mem.h"
#include "include/util/timestamp.h"
#include "include/data/context.h"
#include "include/encryption/randomize.h"

#include <iostream>
namespace data {
    bool context::init(database::mysql::connection& connection, int32_t serverID) {
        serverID_ = serverID;
        return loginSearch_.prepare(connection) && updateServerOfTicket_.prepare(connection);
    }

    int context::getID() const {
        return serverID_;
    }

    int context::getNumClients() const {
        return 0;
    }

    bool context::login(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID, authentication::ticket_t& ticket) {
        if (loginSearch_.search(credentials)) {

            ticket.accountID = loginSearch_.accountID_;
            ticket.serverID = serverID;
            ticket.permissions = loginSearch_.permissions_;
            ticket.issuer = serverID_;
            ticket.loginTime = util::timestamp::now();

            if (!encryption::randomize<authentication::random_key_t>(&ticket.key)) {
                return false;
            }

            authentication::user_data_t& user = userData_[ticket.accountID];

            if (user.ticket.serverID != ticket.serverID) {
                if (!updateServerOfTicket_.update(ticket)) {
                    return false;
                }
            }

            user.ticket = ticket;
            user.credentials = credentials;

            return true;
        }

        return false;
    }

    bool context::logout(const authentication::identification_t& identification) {
        const authentication::user_data_t* user = authenticate(identification);

        if (user != nullptr) {
            // Ignore duplicates
            return userData_.erase(identification.accountID) > 0;
        }

        return false;
    }

    bool context::validateTicket(const authentication::identification_t& identification, const authentication::ticket_t& ticket) {
        const authentication::user_data_t* user = authenticate(identification); //TODO add requiredPermissionMask

        if (user != nullptr) {
            auto result = userData_.find(ticket.accountID);

            if (result != userData_.end()) {
                return util::mem::cmp<authentication::ticket_t>(&(result->second).ticket, &ticket) == 0;
            }
        }

        return false;
    }

    const authentication::user_data_t* context::authenticate(const authentication::identification_t& identification, authentication::permissions_t requiredPermissionMask) const {
        auto result = userData_.find(identification.accountID);

        if (result != userData_.end()) {
            if ((result->second.ticket.permissions & requiredPermissionMask) == requiredPermissionMask && util::mem::cmp<authentication::credentials_t>(&(result->second).credentials, &identification.credentials) == 0) {
                return &result->second;
            }

        }

        return nullptr;
    }
}
