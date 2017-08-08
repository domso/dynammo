#ifndef gameServer_data_map_data_thread_h
#define gameServer_data_map_data_thread_h

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
        class data_thread {
        public:

            struct options_t {

            };

            data_thread();
            ~data_thread();

            data_thread(const data_thread&) = delete;

            bool start(const options_t& options);
            bool stop();
            void mainThread(const options_t& options);
        private:
            std::atomic<bool> m_isRunning;
            std::thread m_mainThreadHandle;
        };
    }
}

#endif



