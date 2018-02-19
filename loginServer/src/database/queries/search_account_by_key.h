#ifndef loginServer_database_queries_search_account_by_key_h
#define loginServer_database_queries_search_account_by_key_h

#include <string>
#include "src/database/prepared_statement.h"
#include "src/authentication/types.h"

/*

Create table keymap (
    accountID int primary key,
    publicKey varchar(128)
);

*/
namespace database {
    namespace queries {
        //______________________________________________________________________________________________________
        //
        // Search for a specific public key in the database
        //______________________________________________________________________________________________________
        class search_account_by_key : public database::mysql::prepared_statement {
        public:
            bool init();
            std::string search_for_key(const authentication::accountID_t& accountID);
        private:
            constexpr static int buffer_size = 1024;
            char m_result[buffer_size];
            authentication::accountID_t m_accountID;
        };
    }
}

#endif


