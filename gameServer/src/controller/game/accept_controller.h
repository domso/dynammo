#ifndef gameServer_controller_game_accept_controller_h
#define gameServer_controller_game_accept_controller_h

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include "network/tcp_socket.h"
#include "network/tcp_connection.h"
#include "src/util/executor.h"
#include "src/authentication/types.h"

namespace controller {
    namespace game {
        class accept_controller {
        public:

            struct options_t {
                uint16_t port;
                int backlog;
                int maxPending;
                float updateIntervall;
            };

            accept_controller(const options_t& options);
            ~accept_controller();

            accept_controller(const accept_controller&) = delete;

            bool start();
            bool stop();
            void main_thread();

            std::shared_ptr<network::tcp_connection<network::ipv4_addr>> get_connection(const authentication::ticket_t ticket);

        private:

            struct local_connection_context_t {
                std::shared_ptr<network::tcp_connection<network::ipv4_addr>> connection;
                std::mutex mutex;
                std::atomic<authentication::ticket_t> ticket;
            };

            options_t m_currentOptions;
            network::tcp_socket<network::ipv4_addr> m_socket;
            std::vector<local_connection_context_t> m_pendingConnections;
            std::atomic<bool> m_isRunning;
            std::thread m_mainThreadHandle;
        };
    }
}

#endif


