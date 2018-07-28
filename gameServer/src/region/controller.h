#ifndef gameServer_region_controller_h
#define gameServer_region_controller_h

#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <atomic>

#include "src/util/lock_ref.h"
#include "src/util/timed_object.h"
#include "src/region/context.h"

namespace region {
    class controller {
    public:
        controller();
        controller(const controller& copy) = delete;
        controller(controller&& copy) = delete;
        ~controller();

        util::locked_ref<region::context> get_region(const uint32_t id);                
        void update();
        void close();
    private:                   
        constexpr static const int secTimeout = 1;
        constexpr static const int msDelay = 2500;
        
        std::optional<uint32_t> next_region_for_update();
        
        std::atomic<bool> m_running;
        std::mutex m_mutex;
        std::condition_variable m_cond;
        std::queue<util::timed_object<uint32_t>> m_updateQueue;
        std::unordered_map<uint32_t, region::context> m_regionMap;
    };
}

#endif
