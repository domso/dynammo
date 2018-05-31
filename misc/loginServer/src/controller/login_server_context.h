#ifndef loginServer_controller_context_h
#define loginServer_controller_context_h

#include "src/message/msg_header.h"
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/database/queries/search_account_by_key.h"

namespace controller {
    //______________________________________________________________________________________________________
    //
    // userdata-layer for server
    //______________________________________________________________________________________________________
    class login_server_context {
    public:
        login_server_context();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - initializes the SQL-statements
        // Parameters:
        // - connection: reference to a valid and open connection to the database
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool init(database::mysql::connection& connection);

        std::string map_id_to_key(const authentication::accountID_t id);
    private:
        database::queries::search_account_by_key m_keySearch;        
    };
}

#endif

