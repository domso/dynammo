#ifndef loginServer_database_queries_search_account_by_key_h
#define loginServer_database_queries_search_account_by_key_h

#include "include/database/prepared_statement.h"
#include "include/authentication/types.h"

/*

Create table user (
accountID int primary key,
serverID int,
username varchar(8) unique,
key varchar(128),
email varchar(100),
permissions int
);

*/
namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific user in the database
        //______________________________________________________________________________________________________
        class search_account_by_key : public database::mysql::prepared_statement {
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
            // - performs a query for an user specified by the given credentials
            // - the results are stored into the permissions_ and accountID_ fields
            // Parameter:
            // - credentials: the userdata for the query
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool search(const authentication::credentials_t& credentials);
            
            authentication::permissions_t permissions_;
            authentication::accountID_t accountID_;
            authentication::credentials_t credentials_;
        };
    }
}

#endif


