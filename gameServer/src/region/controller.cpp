#include "src/region/controller.h"

region::controller::controller(user::controller& userCtrl) : m_userCtrl(userCtrl) {
    
}

void region::controller::open_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, m_userCtrl));
}

void region::controller::close_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionMap.erase(id);
}

bool region::controller::open(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_regionMap.count(id) != 0;
}

util::locked_ref<region::context> region::controller::get_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    if (m_regionMap.count(id) == 0) {
        m_regionMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, m_userCtrl));
    }
    
    util::locked_ref<region::context> result(m_regionMap.at(id));
    return result;
}
