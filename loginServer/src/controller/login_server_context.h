#ifndef loginServer_controller_context_h
#define loginServer_controller_context_h

#include "src/message/msg_header.h"
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/database/queries/search_account_by_key.h"
#include "src/database/queries/update_service_of_user.h"
#include "src/database/queries/register_service.h"
#include "src/database/queries/unregister_service.h"
#include "src/database/queries/search_server_by_id.h"

namespace controller {
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
        // - privateKey: reference to valid private-Key used for signing
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool init(database::mysql::connection& connection, int32_t serverID, encryption::private_key* privateKey = nullptr);
        //______________________________________________________________________________________________________
        //
        // Return:
        // - current local serverID
        //______________________________________________________________________________________________________
        int getID() const;
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to authenticate the user specified by the credentials and stores the session in 'session'
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - session: reference to output instance
        // Return:
        // - message::status::ok  | on success
        // - message::status::error::auth | on an authentication-error
        //______________________________________________________________________________________________________
        message::msg_status_t authenticate(const authentication::credentials_t& credentials, authentication::session_t& session);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to get the destination address for a session
        // Parameters:
        // - session: session, which links to a service
        // - addr: reference to output instance
        // Return:
        // - message::status::ok  | on success
        // - message::status::error::unavailable | if server is not available
        //______________________________________________________________________________________________________
        message::msg_status_t getDestination(const authentication::session_t& session, network::ipv4_addr& addr);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to register the server at 'addr' to the given server-ID
        // Parameters:
        // - credentials: reference to an input instance for the userdata
        // - serverID: ID of new server
        // - addr: address of new server
        // Return:
        // - message::status::ok  | on success
        // - message::status::error::auth | on an authentication-error
        // - message::status::error::execution | on an internal-error
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
        // - message::status::ok  | on success
        // - message::status::error::auth | on an authentication-error
        // - message::status::error::execution | on an internal-error
        //______________________________________________________________________________________________________
        message::msg_status_t unregisterService(const authentication::credentials_t& credentials, const authentication::serverID_t& serverID);
        //______________________________________________________________________________________________________
        //
        // Return:
        // - returns the private-key used for signing
        //______________________________________________________________________________________________________
        encryption::private_key* getPrivateKey() {
            return m_privateKey;
        }
    private:
        int32_t m_serverID;
        database::queries::search_account_by_key m_loginSearch;
        database::queries::update_service_of_user m_updateServerOfTicket;

        database::queries::search_server_by_id m_serverSearch;

        database::queries::register_service m_register;
        database::queries::unregister_service m_unregister;
        
        encryption::private_key* m_privateKey;
    };
}

#endif

