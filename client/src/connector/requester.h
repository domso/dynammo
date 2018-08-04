#pragma once

#include <mutex>

#include "network/pkt_buffer.h"
#include "src/message/msg_controller.h"
#include "src/connector/context.h"

namespace connector {
    class requester {
    public:
        requester(connector::context& context, message::msg_controller& msgCtrl);
        requester(const requester& copy) = delete;
        requester(requester&& move) = delete;
        ~requester();
    private:
        template <typename T>
        static void execute_callback(const types::game_events event, requester* req) {
            std::lock_guard<std::mutex> lg(req->m_mutex);
            req->lazy_init();
            req->m_buffer.clear();
            req->m_msgCtrl.exec_request<T, uint64_t>(req->m_udpDestAddr, req->m_buffer, event);
        }
        
        void lazy_init();

        bool m_open = false;
        std::mutex m_mutex;
        
        constexpr static const auto bufferSize = 1500;
        network::pkt_buffer m_buffer;
        network::ipv4_addr m_udpDestAddr;

        connector::context& m_context;
        message::msg_controller& m_msgCtrl;
    };
}
