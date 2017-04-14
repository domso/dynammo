#include "include/database/queries/search_account_by_key.h"

namespace database {
    namespace queries {
        bool search_account_by_key::init() {
            bool result = true;
            result &= setQuery("Select accountID, permission from user where username = ? and userkey = ?;");
            result &= setResult<authentication::accountID_t>(0, MYSQL_TYPE_LONG, &accountID_);
            result &= setResult<authentication::permissions_t>(1, MYSQL_TYPE_LONG, &permissions_);
            result &= setParam<char>(0,  MYSQL_TYPE_STRING, credentials_.username, sizeof(credentials_.username));
            result &= setParam<char>(1,  MYSQL_TYPE_STRING, credentials_.key, sizeof(credentials_.key));

            result &= bindResult();
            result &= bindParam();

            return result;
        }

        bool search_account_by_key::search(const authentication::credentials_t& credentials) {
            credentials_ = credentials;
            permissions_ = 0;
            accountID_ = 0;
            return execute() && fetchRow();
        }
    }
}




