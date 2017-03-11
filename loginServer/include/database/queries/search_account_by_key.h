#ifndef loginServer_database_queries_search_account_by_key_h
#define loginServer_database_queries_search_account_by_key_h

#include "include/database/prepared_statement.h"
#include "include/authentication/types.h"

/*

Create table userInfo (
accountID int primary key,
username varchar(8) unique,
key varchar(128),
email varchar(100),
permissions int
);

*/
namespace database {
    namespace queries {
        class search_account_by_key : public database::mysql::prepared_statement {
        public:

            bool init();
            bool search(const authentication::credentials_t& credentials);

            authentication::permissions_t permissions_;
            authentication::accountID_t accountID_;
            authentication::credentials_t credentials_;
        };
    }
}

#endif


