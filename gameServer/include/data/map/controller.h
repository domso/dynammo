#ifndef gameServer_data_map_controller_h
#define gameServer_data_map_controller_h

#include <atomic>
#include <vector>
#include <thread>
#include "include/util/executor.h"
#include "network/tcp_connection.h"
#include "include/data/map/send_thread.h"
#include "include/data/map/accept_thread.h"

namespace data {
    namespace map {
        class controller {
        public:
            controller();
            ~controller();

            bool open();
            void close();

        private:
            std::atomic<bool> mapOpen_;

            std::mutex connectionMutex;
            std::queue<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>> connectionQueue;
            
            send_thread sender;
            accept_thread accepter;
        };
    }
}

#endif
