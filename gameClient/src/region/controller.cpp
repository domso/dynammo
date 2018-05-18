#include "src/region/controller.h"

region::controller::controller(graphic::controller& graphicCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl) : m_graphicCtrl(graphicCtrl), m_sessionCtrl(sessionCtrl), m_eventCtrl(eventCtrl) {   
    eventCtrl.register_event_handler<region::controller::event_handler<types::game_events::key_up>>(this);
    eventCtrl.register_event_handler<region::controller::event_handler<types::game_events::key_down>>(this);
    eventCtrl.register_event_handler<region::controller::event_handler<types::game_events::key_left>>(this);
    eventCtrl.register_event_handler<region::controller::event_handler<types::game_events::key_right>>(this);    
}

util::locked_ref<region::context> region::controller::operator[](const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);        
    return try_insert(id);
}

void region::controller::remove(const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionData.erase(id);
}

void region::controller::select_region(const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);        
    m_selectedRegion = id;
    //lazy initialization
}

util::locked_ref<region::context> region::controller::get_selected_region() {
    std::lock_guard<std::mutex> lg(m_mutex);    
    return try_insert(m_selectedRegion);
}

util::locked_ref<region::context> region::controller::try_insert(const uint32_t id) {
    m_regionData.try_emplace(id, id, m_graphicCtrl, m_sessionCtrl);    
    return util::locked_ref<region::context>(m_regionData.at(id));
}

