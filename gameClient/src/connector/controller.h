#pragma once

#include <thread>
#include <mutex>

#include "src/util/event_controller.h"
#include "src/util/config_file.h"
#include "src/types/game_events.h"
#include "src/message/msg_controller.h"
#include "src/connector/tcp_receiver.h"
#include "src/connector/requester.h"
#include "src/connector/data_transfer/context.h"
#include "src/connector/context.h"
#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    class controller {
    public:
        controller(region::controller& regCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl, util::config_file& config);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        bool open();
    private:
        bool load_config();
        
        constexpr static const int bufferSize = 1500;
        
        struct open_event {
            constexpr static const auto trigger = types::game_events::request_login;
            static types::game_events handle(const types::game_events event, const uint64_t& arg, connector::controller* ctrl);
        };

        network::ipv4_addr m_udpDestAddr;
        network::ipv4_addr m_tcpDestAddr;
        util::config_file& m_config;

        std::thread m_msgCtrlThread;
        std::thread m_tcpRecvThread;

        message::msg_controller m_msgCtrl;
        connector::tcp_receiver m_tcpRecv;

        // needs to be initialized last!
        data_transfer::context m_dataContext;
        msg_transfer::context m_messageContext;
        requester m_requester;
    };
}
