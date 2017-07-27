#ifndef loginServer_database_queries_unregisterservice_h
#define loginServer_database_queries_unregisterservice_h

#include "src/database/prepared_statement.h"
#include "src/authentication/types.h"
#include "network/ip_addr.h"


namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific server-address in the database
        //______________________________________________________________________________________________________
        class unregister_service : public database::mysql::prepared_statement {
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
            // - delets the server specified by the given id and the address
            // Parameter:
            // - serverID: the id of the requested server
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool removeService(const authentication::m_serverIDt& serverID);

            authentication::m_serverIDt m_serverID;
            network::ipv4_addr m_addr;
        };
    }
}

#endif


