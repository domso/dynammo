#ifndef loginServer_data_context_h
#define loginServer_data_context_h

#include <unordered_map>
#include "include/authentication/types.h"
#include "include/encryption/rsa.h"
#include "include/database/queries/search_account_by_key.h"
#include "include/database/queries/update_server_of_user.h"

namespace data {
    //______________________________________________________________________________________________________
    //
    // userdata-layer for server
    //______________________________________________________________________________________________________
    class context {
    public:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - initializes the SQL-statements
        // Parameters:
        // - connection: reference to a valid and open connection to the database
        // - serverID: the local serverID of this instance
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool init(database::mysql::connection& connection, int32_t serverID);
        //______________________________________________________________________________________________________
        //
        // Return:
        // - current local serverID
        //______________________________________________________________________________________________________
        int getID() const;
        //______________________________________________________________________________________________________
        //
        // Return:
        // - current number of connected clients
        //______________________________________________________________________________________________________
        int getNumClients() const;
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to perform an login for the user specified by the credentials on a non-login-server
        // - stored the created ticket in the given 'ticket'-argument
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - serverID: reference to requested serverID
        // - ticket: reference to an output instance for the ticket
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool login(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID, authentication::ticket_t& ticket);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to perform an logout for the user specified by the identification
        // Parameters:
        // - identification: reference to an input instance for the userdata
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool logout(const authentication::identification_t& identification);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to validate a user by his ticket
        // - requestor and ticket-owner are not the same user!
        // Parameters:
        // - identification: reference to an input instance for the requestor-userdata
        // - ticket: reference to an input instance for the ticket itself
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool validateTicket(const authentication::identification_t& identification, const authentication::ticket_t& ticket);
    private:
        const authentication::user_data_t* authenticate(const authentication::identification_t& identification, authentication::permissions_t requiredPermissionMask = 0) const;
        int32_t serverID_;
        database::queries::search_account_by_key loginSearch_;
        database::queries::update_server_of_user updateServerOfTicket_;
        std::unordered_map<authentication::accountID_t, authentication::user_data_t> userData_;
    };
}

#endif
