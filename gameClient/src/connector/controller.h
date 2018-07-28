#pragma once

#include "src/message/msg_controller.h"
#include "src/connector/requester.h"
#include "src/connector/context.h"

namespace connector {
    class controller {
    public:
        controller(connector::context& context);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        void update();
        void close();
    private:
        constexpr static const int bufferSize = 1500;

        connector::context& m_context;
        message::msg_controller m_msgCtrl;
        requester m_requester;
    };
}
