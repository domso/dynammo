#ifndef loginServer_database_queries_register_service_h
#define loginServer_database_queries_register_service_h

#include "include/database/prepared_statement.h"
#include "include/authentication/types.h"
#include "network/ip_addr.h"


namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific server-address in the database
        //______________________________________________________________________________________________________
        class register_service : public database::mysql::prepared_statement {
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
            // - inserts the server specified by the given id and the address
            // Parameter:
            // - serverID: the id of the requested server
            // - addr: address of server for the new service
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool addService(const authentication::serverID_t& serverID, const network::ipv4_addr& addr);

            authentication::serverID_t serverID_;
            network::ipv4_addr addr_;
        };
    }
}

#endif


