#pragma once

#include <mutex>

#include "src/authentication/types.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"

namespace session {
    class controller {
    public:
        controller(util::event_controller<types::game_events>& eventCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;

        void set_tcp_link(authentication::ticket_t newLink);
        authentication::ticket_t get_tcp_link();
    private:
        std::mutex m_mutex;
        util::event_controller<types::game_events>& m_eventCtrl;
        authentication::ticket_t m_tcpTicket;
    };
}
