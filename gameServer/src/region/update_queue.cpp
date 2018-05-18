#include "src/region/update_queue.h"

region::update_queue::update_queue() {
}

void region::update_queue::insert(const timed_obj newObj) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_queue.push(newObj);
    m_cond.notify_all();
}

void region::update_queue::insert(const uint32_t newObj) {
    insert(timed_obj(newObj));
}

bool region::update_queue::wait_and_get(uint32_t* obj) {
    std::unique_lock<std::mutex> ul(m_mutex);

    while (m_queue.empty()) {
        std::chrono::duration<int> duration(secTimeout);
        if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
            return false;
        }
    }
    
    *obj = m_queue.wait_front(msDelay);
    m_queue.pop();
    
    return true;
}

