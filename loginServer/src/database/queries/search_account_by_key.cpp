#include "src/database/queries/search_account_by_key.h"

namespace database {
    namespace queries {
        bool search_account_by_key::init() {
            bool result = true;
            result &= setQuery("Select publicKey from keymap where accountID = ?;");
            result &= setParam<authentication::accountID_t>(0, MYSQL_TYPE_LONG, &m_accountID);
            result &= setResult<char[buffer_size]>(0,  MYSQL_TYPE_STRING, &m_result, sizeof(m_result));

            result &= bindResult();
            result &= bindParam();

            return result;
        }

        std::string search_account_by_key::search_for_key(const authentication::accountID_t& accountID) {
            m_accountID = accountID;

            if (execute() && fetchRow()) {
                return std::move(std::string(m_result));
            }           
            
            return std::string();
        }
    }
}




