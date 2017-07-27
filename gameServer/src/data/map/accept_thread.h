#ifndef gameServer_data_map_accept_thread_h
#define gameServer_data_map_accept_thread_h

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include "network/tcp_socket.h"
#include "network/tcp_connection.h"
#include "src/util/executor.h"
#include "src/data/map/send_thread.h"

namespace data {
    namespace map {
        class accept_thread {
        public:   
            
            struct options_t {
                std::mutex* mutex;
                std::queue<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>>* connectionQueue;
                send_thread* sender;
                uint16_t port;
                int backlog;
                float updateIntervall;
            };
            
            accept_thread();
            ~accept_thread();
            
            accept_thread(const accept_thread&) = delete;
            
            bool start(const options_t& options);
            bool stop();
            void mainThread(const options_t& options);
        private:
            network::tcp_socket<network::ipv4_addr> m_socket;
            std::atomic<bool> m_isRunning;
            std::thread m_mainThreadHandle;
        };
    }
}

#endif


