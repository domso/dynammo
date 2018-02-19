#include "src/database/queries/update_service_of_user.h"

namespace database {
    namespace queries {
        bool update_service_of_user::init() {
            bool result = true;
            result &= setQuery("update user set serverID=? where accountID=?;");
            result &= setParam<authentication::serverID_t>(0,  MYSQL_TYPE_LONG, &m_serverID, 1);
            result &= setParam<authentication::accountID_t>(1,  MYSQL_TYPE_LONG, &m_accountID, 1);

            result &= bindParam();

            return result;
        }

        bool update_service_of_user::update(const authentication::session_t& session) {
            m_accountID = session.accountID;
            m_serverID = session.serverID;
            return execute();
        }

    }
}




