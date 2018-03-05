#pragma once

#include <thread>
#include <mutex>

#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/message/msg_controller.h"
#include "src/connector/tcp_receiver.h"
#include "src/connector/requester.h"
#include "src/connector/data_transfer/context.h"
#include "src/connector/msg_transfer/context.h"
#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    class controller {
    public:
        controller(network::ipv4_addr& tcpDestAddr, network::ipv4_addr& udpDestAddr, region::controller& regCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        bool open();
    private:
        constexpr static const int bufferSize = 1500;

        bool m_status;

        network::ipv4_addr m_udpDestAddr;
        network::ipv4_addr m_tcpDestAddr;

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
