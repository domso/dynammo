#ifndef loginServer_database_queries_unregister_service_h
#define loginServer_database_queries_unregister_service_h

#include "include/database/prepared_statement.h"
#include "include/authentication/types.h"
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
            bool removeService(const authentication::serverID_t& serverID);

            authentication::serverID_t serverID_;
            network::ipv4_addr addr_;
        };
    }
}

#endif


