#ifndef gameServer_connector_worker_h
#define gameServer_connector_worker_h

#include <thread>
#include <atomic>

#include "src/message/msg_controller.h"
#include "src/connector/context.h"


namespace connector {
    class worker {
    public:
        enum types {
            receiver,
            updater
        };
                
        worker(connector::context& context);
        worker(connector::worker& copy) = delete;
        ~worker();
        
        void init_as(const types t);
    private:                
        constexpr static const int bufferSize = 1500;
        
        void init_as_receiver();
        void init_as_updater();      
        
        void update_thread_main();
        
        connector::context& m_context;
        message::msg_controller m_msgCtrl;
        std::thread m_thread;
        std::atomic<bool> m_running;
    };
}

#endif
