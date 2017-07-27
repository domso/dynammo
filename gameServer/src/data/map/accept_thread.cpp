#include "src/data/map/accept_thread.h"

namespace data {
    namespace map {
        accept_thread::accept_thread() {

        }

        accept_thread::~accept_thread() {
            if (m_mainThreadHandle.joinable()) {
                m_mainThreadHandle.join();
            }
        }

        bool accept_thread::start(const options_t& options) {
            bool current = false;

            if (m_isRunning.compare_exchange_strong(current, true)) {
                if (!m_socket.acceptOn(options.port, options.backlog)) {
                    return false;
                }

                m_socket.setTimeout(options.updateIntervall);

                if (m_mainThreadHandle.joinable()) {
                    m_mainThreadHandle.join();
                }
                
                m_mainThreadHandle = std::thread(&accept_thread::mainThread, this, std::ref(options));
                
                return true;
            }
            
            return false;
        }

        bool accept_thread::stop() {
            bool current = true;
            return m_isRunning.compare_exchange_strong(current, false);
        }

        void accept_thread::mainThread(const options_t& options) {
            while (m_isRunning.load()) {
                auto connection = m_socket.acceptConnection();
                if (connection != nullptr) {
                    std::lock_guard<std::mutex> lg(*options.mutex);

                    options.connectionQueue->push(connection);
                    options.sender->executor.invoke<send_thread::addConnectionTask>();
                }
            }
        }
    }
}

