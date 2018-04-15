#pragma once

#include <mutex>

#include "network/pkt_buffer.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/message/msg_controller.h"
#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    class requester {
    public:
        requester(network::ipv4_addr& udpDestAddr, util::event_controller<types::game_events>& eventCtrl, message::msg_controller& msgCtrl);
        requester(const requester& copy) = delete;
        requester(requester&& move) = delete;
        ~requester();
        
        void open();
        void close();
    private:
        template <types::game_events triggerT, typename T>
        struct event_handler {
            constexpr static const auto trigger = triggerT;
            static types::game_events handle(const types::game_events event, requester* req) {
                std::lock_guard<std::mutex> lg(req->m_mutex);
                if (req->m_open) {
                    req->execute<T>(event);
                }
                return types::game_events::clear;
            }
        };

        template <typename T>
        void execute(const types::game_events event) {
            m_buffer.clear();
            m_msgCtrl.exec_request<T, types::game_events>(m_udpDestAddr, m_buffer, event);
        }

        template <typename T>
        typename T::content& content() {
            return *(m_msgCtrl.additional_datatype<T>());
        }

        bool m_open = false;
        std::mutex m_mutex;
        constexpr static const auto bufferSize = 1500;
        network::pkt_buffer m_buffer;
        network::ipv4_addr& m_udpDestAddr;

        util::event_controller<types::game_events>& m_eventCtrl;
        message::msg_controller& m_msgCtrl;
    };
}
