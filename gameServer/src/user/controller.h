#ifndef gameServer_user_controller_h
#define gameServer_user_controller_h

#include <mutex>
#include <unordered_map>
#include "src/authentication/types.h"
#include "src/util/lock_ref.h"
#include "src/user/info.h"

namespace user {
    class controller {
    public:
        controller();
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        
        util::locked_ref<info> get_info(const authentication::accountID_t id);
        void remove(const authentication::accountID_t id);
    private:
        std::mutex m_mutex;
        std::unordered_map<authentication::accountID_t, info> m_userInfo; 
    };
}

#endif
