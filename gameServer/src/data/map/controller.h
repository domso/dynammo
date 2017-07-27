#ifndef gameServer_data_map_controller_h
#define gameServer_data_map_controller_h

#include <atomic>
#include <vector>
#include <thread>
#include "src/util/executor.h"
#include "network/tcp_connection.h"
#include "src/data/map/send_thread.h"
#include "src/data/map/accept_thread.h"

namespace data {
    namespace map {
        class controller {
        public:
            controller();
            ~controller();

            bool open();
            void close();

        private:
            std::atomic<bool> m_mapOpen;

            std::mutex connectionMutex;
            std::queue<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>> connectionQueue;
            
            send_thread sender;
            accept_thread accepter;
        };
    }
}

#endif
