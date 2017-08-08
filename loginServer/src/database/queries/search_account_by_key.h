#ifndef loginServer_database_queries_search_account_by_key_h
#define loginServer_database_queries_search_account_by_key_h

#include "src/database/prepared_statement.h"
#include "src/authentication/types.h"

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
            // - the results are stored into the m_permissions and m_accountID fields
            // Parameter:
            // - credentials: the userdata for the query
            // Return:
            // - true  | on success
            // - false | on any error
            //______________________________________________________________________________________________________
            bool search(const authentication::credentials_t& credentials);

            authentication::permissions_t m_permissions;
            authentication::accountID_t m_accountID;
            authentication::serverID_t m_serverID;
            authentication::credentials_t m_credentials;
        };
    }
}

#endif


