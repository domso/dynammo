#include "src/database/queries/update_service_of_user.h"

namespace database {
    namespace queries {
        bool update_service_of_user::init() {
            bool result = true;
            result &= setQuery("update user set serverID=? where accountID=?;");
            result &= setParam<authentication::m_serverIDt>(0,  MYSQL_TYPE_LONG, &m_serverID, 1);
            result &= setParam<authentication::m_accountIDt>(1,  MYSQL_TYPE_LONG, &m_accountID, 1);

            result &= bindParam();

            return result;
        }

        bool update_service_of_user::update(const authentication::ticket_t& ticket) {
            m_accountID = ticket.accountID;
            m_serverID = ticket.serverID;
            return execute();
        }

    }
}




