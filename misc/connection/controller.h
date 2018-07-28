#ifndef gameServer_connection_controller_h
#define gameServer_connection_controller_h

#include <thread>
#include "worker.h"

namespace connection {
    class controller {
    public:
        controller(const uint16_t port);
        controller(const controller& copy) = delete;
        controller(controller& move) = delete;
        ~controller();

        network::tcp_connection<network::ipv4_addr> get_new_connection(const int id);
        
        void close();
    private:
        std::thread m_workerThread;
        worker m_worker;
    };
}

#endif
