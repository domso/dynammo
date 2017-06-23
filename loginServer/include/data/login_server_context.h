#ifndef loginServer_data_context_h
#define loginServer_data_context_h

#include "include/message/msg_header.h"
#include "include/authentication/types.h"
#include "include/encryption/rsa.h"
#include "include/database/queries/search_account_by_key.h"
#include "include/database/queries/update_service_of_user.h"
#include "include/database/queries/register_service.h"
#include "include/database/queries/unregister_service.h"
#include "include/database/queries/search_server_by_id.h"

namespace data {
    //______________________________________________________________________________________________________
    //
    // userdata-layer for server
    //______________________________________________________________________________________________________
    class login_server_context {
    public:
        //______________________________________________________________________________________________________
        //
        // Parameters:
        // - maxBufferSize: size of internal buffer
        //______________________________________________________________________________________________________
        login_server_context(const int maxBufferSize);
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
        // Description:
        // - tries to authenticate the user specified by the credentials and stores the ticket in 'ticket'
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - ticket: reference to output instance
        // Return:
        // - MSG_STATUS_OK  | on success
        // - MSG_STATUS_AUTH_ERROR | on an authentication-error
        //______________________________________________________________________________________________________
        message::msg_status_t authenticate(const authentication::credentials_t& credentials, authentication::ticket_t& ticket);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to get the destination address for a ticket
        // Parameters:
        // - ticket: ticket, which links to a service
        // - addr: reference to output instance
        // Return:
        // - MSG_STATUS_OK  | on success
        // - MSG_STATUS_UNAVAILABLE | if server is not available
        //______________________________________________________________________________________________________
        message::msg_status_t getDestination(const authentication::ticket_t& ticket, network::ipv4_addr& addr);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to register the server at 'addr' to the given server-ID
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - serverID: ID of new server
        // - addr: address of new server
        // Return:
        // - MSG_STATUS_OK  | on success
        // - MSG_STATUS_AUTH_ERROR | on an authentication-error
        // - MSG_STATUS_EXECUTION_ERROR | on an internal-error
        //______________________________________________________________________________________________________
        message::msg_status_t registerService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID, const network::ipv4_addr& addr);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to unregister the server specified by his server-ID
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - serverID: ID of old server
        // Return:
        // - MSG_STATUS_OK  | on success
        // - MSG_STATUS_AUTH_ERROR | on an authentication-error
        // - MSG_STATUS_EXECUTION_ERROR | on an internal-error
        //______________________________________________________________________________________________________
        message::msg_status_t unregisterService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID);
    private:
        int32_t serverID_;
        database::queries::search_account_by_key loginSearch_;
        database::queries::update_service_of_user updateServerOfTicket_;

        database::queries::search_server_by_id serverSearch_;

        database::queries::register_service register_;
        database::queries::unregister_service unregister_;
    };
}

#endif

