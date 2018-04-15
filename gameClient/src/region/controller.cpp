#include "src/region/controller.h"

region::controller::controller(graphic::controller& graphicCtrl, session::controller& sessionCtrl) : m_graphicCtrl(graphicCtrl), m_sessionCtrl(sessionCtrl) {
    
}

util::locked_ref<region::context> region::controller::operator[](const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);        
    m_regionData.try_emplace(id, m_graphicCtrl, m_sessionCtrl);
    
    return util::locked_ref<region::context>(m_regionData.at(id));
}

void region::controller::remove(const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionData.erase(id);
}

