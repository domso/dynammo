#include "src/database/queries/search_account_by_key.h"

namespace database {
    namespace queries {
        bool search_account_by_key::init() {
            bool result = true;
            result &= setQuery("Select accountID, permission, serverID from user where username = ? and userkey = ?;");
            result &= setResult<authentication::m_accountIDt>(0, MYSQL_TYPE_LONG, &m_accountID);
            result &= setResult<authentication::m_permissionst>(1, MYSQL_TYPE_LONG, &m_permissions);
            result &= setResult<authentication::m_serverIDt>(2, MYSQL_TYPE_LONG, &m_serverID);
            result &= setParam<char>(0,  MYSQL_TYPE_STRING, m_credentials.username, sizeof(m_credentials.username));
            result &= setParam<char>(1,  MYSQL_TYPE_STRING, m_credentials.key, sizeof(m_credentials.key));

            result &= bindResult();
            result &= bindParam();

            return result;
        }

        bool search_account_by_key::search(const authentication::credentials_t& credentials) {
            m_credentials = credentials;
            m_permissions = 0;
            m_accountID = 0;
            m_serverID = 0;

            return execute() && fetchRow();
        }
    }
}




