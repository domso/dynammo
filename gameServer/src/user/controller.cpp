#include "src/user/controller.h"

user::controller::controller() {
    
}

util::locked_ref<user::info> user::controller::get_info(const authentication::accountID_t id) {
    return util::locked_ref<user::info>(m_userInfo[id]);
}

void user::controller::remove(const authentication::accountID_t id) {
    m_userInfo.erase(id);
}
