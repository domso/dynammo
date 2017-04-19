#include "include/database/queries/update_service_of_user.h"

namespace database {
    namespace queries {
        bool update_service_of_user::init() {
            bool result = true;
            result &= setQuery("update user set serverID=? where accountID=?;");
            result &= setParam<authentication::serverID_t>(0,  MYSQL_TYPE_LONG, &serverID_, 1);
            result &= setParam<authentication::accountID_t>(1,  MYSQL_TYPE_LONG, &accountID_, 1);

            result &= bindParam();

            return result;
        }

        bool update_service_of_user::update(const authentication::ticket_t& ticket) {
            accountID_ = ticket.accountID;
            serverID_ = ticket.serverID;
            return execute();
        }
        
    }
}




