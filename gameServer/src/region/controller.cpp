#include "src/region/controller.h"

region::controller::controller() {
    
}

void region::controller::open_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionMap.emplace(id, id);
    m_regionUpdateQueue.insert(id);
}

void region::controller::close_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_regionMap.erase(id);
}

bool region::controller::is_open(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_regionMap.count(id) != 0;
}

util::locked_ref<region::context> region::controller::get_region(const uint64_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    if (m_regionMap.count(id) == 0) {
    m_regionMap.emplace(id, id);
        m_regionUpdateQueue.insert(id);
    }
    
    util::locked_ref<region::context> result(m_regionMap.at(id));
    return result;
}

void region::controller::update() {
    uint64_t current;
    if (m_regionUpdateQueue.wait_and_get(&current)) {
        update_queue::timed_obj tObj(current);
        auto region = get_region(current);
        if (region->update()) {
            m_regionUpdateQueue.insert(tObj);
        } else {
            close_region(current);
        }
    }
}

