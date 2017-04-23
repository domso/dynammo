#include "include/database/queries/register_service.h"

namespace database {
    namespace queries {
        bool register_service::init() {
            bool result = true;
            result &= setQuery("insert into services values(?, ?) on duplicate key update addr=?;");
            result &= setParam<authentication::serverID_t>(0,  MYSQL_TYPE_LONG, &serverID_);
            result &= setParam<network::ipv4_addr>(1,  MYSQL_TYPE_BLOB, &addr_);
            result &= setParam<network::ipv4_addr>(2,  MYSQL_TYPE_BLOB, &addr_);

            result &= bindParam();

            return result;
        }

        bool register_service::addService(const authentication::serverID_t& serverID, const network::ipv4_addr& addr) {
            serverID_ = serverID;
            addr_ = addr;
            return execute();
        }
    }
}




