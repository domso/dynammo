#ifndef loginServer_database_queries_update_server_of_user_h
#define loginServer_database_queries_update_server_of_user_h

#include "src/database/prepared_statement.h"
#include "src/authentication/types.h"

namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific user in the database
        //______________________________________________________________________________________________________
        class update_service_of_user : public database::mysql::prepared_statement {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - initializes the SQL-statements
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool init();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - updates the current serverID for an user specified by the given ticket
            // Parameter:
            // - ticket: ticket containing the new serverID and the accountID;
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool update(const authentication::ticket_t& ticket);
        private:
            authentication::m_accountIDt m_accountID;
            authentication::m_serverIDt m_serverID;
        };
    }
}

#endif


