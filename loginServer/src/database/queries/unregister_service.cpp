#include "src/database/queries/unregister_service.h"

namespace database {
    namespace queries {
        bool unregister_service::init() {
            bool result = true;
            result &= setQuery("delete from services where serviceID=?;");
            result &= setParam<authentication::serverID_t>(0,  MYSQL_TYPE_LONG, &m_serverID);

            result &= bindParam();

            return result;
        }

        bool unregister_service::removeService(const authentication::serverID_t& serverID) {
            m_serverID = serverID;
            return execute() && fetchRow();
        }
    }
}




