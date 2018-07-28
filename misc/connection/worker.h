#ifndef gameServer_connection_worker_h
#define gameServer_connection_worker_h

#include <vector>
#include <atomic>
#include <mutex>

#include "network/tcp_connection.h"
#include "network/tcp_socket.h"

namespace connection {
    class worker {
    public:
        worker();
        worker(const worker& copy) = delete;
        worker(const worker&& move) = delete;        
        ~worker();
        
        void accept_new_connections(const uint16_t port);
        network::tcp_connection<network::ipv4_addr> get_connection(const int i);        
        
        bool error() const;
        void close();
    private:
        constexpr static const int max_number_of_pending_connections = 256;
        
        std::atomic<bool> m_error;
        std::atomic<bool> m_running;
        
        std::vector<std::mutex> m_mutexes;
        std::vector<network::tcp_connection<network::ipv4_addr>> m_connections;
    };
}

#endif
