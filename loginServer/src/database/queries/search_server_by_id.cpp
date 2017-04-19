#include "include/database/queries/search_server_by_id.h"

namespace database {
    namespace queries {
        bool search_server_by_id::init() {
            bool result = true;
            result &= setQuery("Select addr from services where serviceID = ?;");
            result &= setResult<network::ipv4_addr>(0, MYSQL_TYPE_BLOB, &addr_);
            result &= setParam<authentication::serverID_t>(0,  MYSQL_TYPE_LONG, &serverID_);

            result &= bindResult();
            result &= bindParam();

            return result;
        }

        bool search_server_by_id::search(const authentication::serverID_t& serverID) {
            serverID_ = serverID;
            return execute() && fetchRow();
        }
    }
}




