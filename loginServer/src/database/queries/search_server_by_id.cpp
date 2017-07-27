#include "src/database/queries/search_server_by_id.h"

namespace database {
    namespace queries {
        bool search_server_by_id::init() {
            bool result = true;
            result &= setQuery("Select addr from services where serviceID = ?;");
            result &= setResult<network::ipv4_addr>(0, MYSQL_TYPE_BLOB, &m_addr);
            result &= setParam<authentication::m_serverIDt>(0,  MYSQL_TYPE_LONG, &m_serverID);

            result &= bindResult();
            result &= bindParam();

            return result;
        }

        bool search_server_by_id::search(const authentication::m_serverIDt& serverID) {
            m_serverID = serverID;
            return execute() && fetchRow();
        }
    }
}




