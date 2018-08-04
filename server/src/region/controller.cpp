#include "src/region/controller.h"

region::controller::controller() {
    m_running = false;
}

region::controller::~controller() {
    assert(m_running == false);
}


util::locked_ref<region::context> region::controller::get_region(const uint32_t id) {
    std::lock_guard<std::mutex> lg(m_mutex);

    if (m_regionMap.count(id) == 0) {
        m_regionMap.emplace(id, id);
        util::timed_object tobj(id);
        m_updateQueue.push(tobj);
        m_cond.notify_one();
    }

    util::locked_ref<region::context> result(m_regionMap.at(id));
    return result;
}

void region::controller::update() {
    m_running = true;
    while (m_running) {
        auto next = next_region_for_update();

        if (next) {
            bool updateResult;
            {
                auto region = get_region(*next);
                updateResult = region->update();
            }
            {
                std::lock_guard<std::mutex> lg(m_mutex);

                if (updateResult) {
                    util::timed_object tobj(*next);
                    m_updateQueue.push(tobj);
                } else {
                    m_regionMap.erase(*next);
                }
            }
        }
    }
}

void region::controller::close() {
    m_running = false;
}


std::optional<uint32_t> region::controller::next_region_for_update() {
    std::unique_lock<std::mutex> ul(m_mutex);

    while (m_updateQueue.empty()) {
        std::chrono::duration<int> duration(secTimeout);

        if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
            return std::nullopt;
        }
    }

    auto tobj = m_updateQueue.front();
    m_updateQueue.pop();
    ul.unlock();
    tobj.wait_till_age(msDelay);

    return tobj.obj;
}


