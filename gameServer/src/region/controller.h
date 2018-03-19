#ifndef gameServer_region_controller_h
#define gameServer_region_controller_h

#include <unordered_map>
#include <mutex>
#include "src/util/lock_ref.h"
#include "src/region/context.h"
#include "src/region/update_queue.h"

namespace region {
    class controller {
    public:
        controller();
        controller(const controller& copy) = delete;
        controller(controller&& copy) = delete;
        
        void open_region(const uint64_t id);
        bool is_open(const uint64_t id);
        util::locked_ref<region::context> get_region(const uint64_t id);        
        
        void update();
    private:        
        void close_region(const uint64_t id);
        
        std::mutex m_mutex;
        update_queue m_regionUpdateQueue;
        std::unordered_map<uint64_t, region::context> m_regionMap;
    };
}

#endif
