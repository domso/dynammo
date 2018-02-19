#ifndef gameServer_connector_worker_h
#define gameServer_connector_worker_h

#include <thread>
#include "src/message/msg_controller.h"
#include "src/connector/context.h"

namespace connector {
    class worker {
    public:
        worker(connector::context& context);
        worker(connector::worker& copy) = delete;
        ~worker();
    private:                
        constexpr static const int bufferSize = 1500;
        
        message::msg_controller m_msgCtrl;
        std::thread m_thread;
    };
}

#endif
