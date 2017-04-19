#ifndef loginServer_database_queries_search_server_by_id_h
#define loginServer_database_queries_search_server_by_id_h

#include "include/database/prepared_statement.h"
#include "include/authentication/types.h"
#include "network/ip_addr.h"

/*

Create table services (
serviceID int primary key,
addr blob
);

*/
namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific server-address in the database
        //______________________________________________________________________________________________________
        class search_server_by_id : public database::mysql::prepared_statement {
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
            // - performs a query for an server specified by the given id
            // - the results are stored into the addr_ field
            // Parameter:
            // - serverID: the id of the requested server
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool search(const authentication::serverID_t& serverID);
            
            authentication::serverID_t serverID_;
            network::ipv4_addr addr_;
        };
    }
}

#endif


