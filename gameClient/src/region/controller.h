#ifndef gameClient_region_controller_h
#define gameClient_region_controller_h

#include <unordered_map>
#include <mutex>
#include "src/util/lock_ref.h"
#include "src/region/context.h"
#include "src/session/controller.h"

namespace region {
    class controller {
    public:
        controller(graphic::controller& graphicCtrl, session::controller& sessionCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        
        util::locked_ref<region::context> operator[](const uint32_t id);
        void remove(const uint32_t id);
    private:
        graphic::controller& m_graphicCtrl;
        session::controller& m_sessionCtrl;
        std::unordered_map<uint32_t, region::context> m_regionData;
        std::mutex m_mutex;
    };
}

#endif
