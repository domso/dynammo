#pragma once

#include <atomic>

#include "src/message/msg_controller.h"
#include "src/connector/context.h"

namespace connector {
    class controller {
    public:
        controller(region::controller& rCtrl, session::controller& sCtrl);
        ~controller();

        class input {
        public:
            input(connector::context& context, message::msg_controller& msgCtrl);
            ~input();

            void update();
            void close();
        private:            
            connector::context& m_context;
            message::msg_controller& m_msgCtrl;
        };

        class output {
        public:
            output(connector::context& context, message::msg_controller& msgCtrl);
            ~output();

            void update();
            void close();
        private:
            std::atomic<bool> m_running;
            connector::context& m_context;
            message::msg_controller& m_msgCtrl;
            network::pkt_buffer m_buffer;
        };
    private:
        connector::context m_context;
        message::msg_controller m_msgCtrl;
    public:
        input inputCtrl;
        output outputCtrl;
    };
}
