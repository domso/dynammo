#include "src/authentication/types.h"
#include "src/util/mem.h"
#include "src/util/timestamp.h"
#include "src/controller/login_server_context.h"
#include "src/encryption/randomize.h"
#include "src/message/msg_controller.h"

namespace controller {
    login_server_context::login_server_context() {

    }

    bool login_server_context::init(database::mysql::connection& connection) {
        return m_keySearch.prepare(connection);
    }

    std::string login_server_context::map_id_to_key(const authentication::accountID_t id) {
        return m_keySearch.search_for_key(id);
    }
}
