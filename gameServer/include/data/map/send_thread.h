#ifndef gameServer_data_map_send_thread_h
#define gameServer_data_map_send_thread_h

#include <queue>
#include <unordered_set>
#include <mutex>
#include "network/tcp_connection.h"
#include "include/util/executor.h"

namespace data {
    namespace map {
        class send_thread {
        public:
            send_thread();

            send_thread(const send_thread&) = delete;
            
            struct context_arg_t {
                std::mutex* mutex;
                std::queue<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>>* connectionQueue;
            };

            struct context_t {
                context_t(const context_arg_t& arg);
                std::mutex& mutex;
                std::queue<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>>& connectionQueue;
                std::unordered_set<std::shared_ptr<network::tcp_connection<network::ipv4_addr>>> connections;
            };

            class addConnectionTask {
            public:
                constexpr static int8_t id = 1;
                static void execute(context_t& context);
            };

            class sendUpdateTask {
            public:
                constexpr static int8_t id = 2;
                static void execute(context_t& context);
            };

            util::executor<context_t, context_arg_t> executor;
        };
    }
}

#endif

