
#include "src/region/context.h"

region::context::context(const uint64_t id, user::controller& userCtrl) : m_id(id), m_userCtrl(userCtrl) {
    load();
}

region::context::~context() {
    save();
}

void region::context::add_user(const authentication::accountID_t accountID) {
    m_activeUsers.insert(accountID);
    auto info = m_userCtrl.get_info(accountID);
    if (!info->connection.is_closed()) {
        m_regionData.send_init(info->connection);
    }
}

void region::context::remove_user(const authentication::accountID_t accountID) {
    m_activeUsers.erase(accountID);
}

void region::context::load() {
    std::cout << "load region #" << m_id << std::endl;
    m_regionData.load(m_id);
}

void region::context::save() {
    std::cout << "save region #" << m_id << std::endl;
    m_regionData.save();
}

void region::context::update() {
    //m_regionData.update();
}
