#pragma once

#include <atomic>

#include "src/message/msg_controller.h"
#include "src/connector/context.h"

namespace connector {
    class controller {
    public:
        controller(region::controller& rCtrl, session::controller& sCtrl);
        ~controller();
        
        void update();
        void close();
    private:
        std::atomic<bool> m_running;
        connector::context m_context;
        message::msg_controller m_msgCtrl;        
    };
}
