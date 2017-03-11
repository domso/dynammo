#ifndef loginServer_data_context_h
#define loginServer_data_context_h

#include <unordered_map>
#include "include/authentication/types.h"
#include "include/encryption/rsa.h"
#include "include/database/queries/search_account_by_key.h"

namespace data {
    class context {
    public:

        bool init(database::mysql::connection& connection, int32_t serverID);

        int getID() const;

        int getNumClients() const;

        bool login(const authentication::credentials_t& credentials, authentication::ticket_t& ticket);

        bool logout(const authentication::identification_t& identification);

        bool validateTicket(const authentication::identification_t& identification, const authentication::ticket_t& ticket);

    private:

        const authentication::user_data_t* authenticate(const authentication::identification_t& identification, authentication::permissions_t requiredPermissionMask = 0) const;

        int32_t serverID_;
        database::queries::search_account_by_key loginSearch;

        std::unordered_map<authentication::accountID_t, authentication::user_data_t> userData_;
    };
}

#endif
