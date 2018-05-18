#pragma once

#include <stdint.h>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "src/util/timed_queue.h"

namespace region {
    class update_queue {
    public:
        update_queue();
        update_queue(const update_queue& copy) = delete;
        update_queue(update_queue&& move) = delete;
                
        typedef util::timed_queue<uint32_t>::itemT timed_obj;
        
        void insert(const timed_obj newObj);        
        void insert(const uint32_t newObj);     
        bool wait_and_get(uint32_t* obj);        
    private:
        constexpr static const int msDelay = 2500;
        constexpr static const int secTimeout = 1;
        
        std::mutex m_mutex;
        std::condition_variable m_cond;
        util::timed_queue<uint32_t> m_queue;
    };
}
