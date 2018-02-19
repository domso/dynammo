#ifndef gameServer_region_controller_h
#define gameServer_region_controller_h

#include <unordered_map>
#include <mutex>
#include "src/util/lock_ref.h"
#include "src/region/context.h"


namespace region {
    class controller {
    public:
        controller(user::controller& userCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& copy) = delete;
        
        void open_region(const uint64_t id);
        void close_region(const uint64_t id);

        bool open(const uint64_t id);
        util::locked_ref<region::context> get_region(const uint64_t id);        
    private:
        std::mutex m_mutex;
        user::controller& m_userCtrl;
        std::unordered_map<uint64_t, region::context> m_regionMap;
    };
}

#endif
