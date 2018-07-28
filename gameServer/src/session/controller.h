#pragma once

#include <mutex>
#include <unordered_map>
#include <atomic>
#include <optional>

#include "src/authentication/types.h"
#include "src/util/lock_ref.h"
#include "src/session/user.h"

namespace session {
    class controller {
    public:
        controller();
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();
        
        std::optional<util::locked_ref<user>> get_user(const uint32_t sessionID);
        void remove(const uint32_t sessionID);
        
        void close();
        void update();
    private:
        void clean();
        uint32_t next_session_id();
        std::atomic<bool> m_running;
        
        uint32_t m_currentSessionID;
        std::mutex m_mutex;
        std::unordered_map<uint32_t, user> m_userInfo; 
    };
}

