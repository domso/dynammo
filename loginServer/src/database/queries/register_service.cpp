#include "src/database/queries/register_service.h"

namespace database {
    namespace queries {
        bool register_service::init() {
            bool result = true;
            result &= setQuery("insert into services values(?, ?) on duplicate key update addr=?;");
            result &= setParam<authentication::m_serverIDt>(0,  MYSQL_TYPE_LONG, &m_serverID);
            result &= setParam<network::ipv4_addr>(1,  MYSQL_TYPE_BLOB, &m_addr);
            result &= setParam<network::ipv4_addr>(2,  MYSQL_TYPE_BLOB, &m_addr);

            result &= bindParam();

            return result;
        }

        bool register_service::addService(const authentication::m_serverIDt& serverID, const network::ipv4_addr& addr) {
            m_serverID = serverID;
            m_addr = addr;
            return execute();
        }
    }
}




