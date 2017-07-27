#include "include/data/map/accept_thread.h"

namespace data {
    namespace map {
        accept_thread::accept_thread() {

        }

        accept_thread::~accept_thread() {
            if (mainThreadHandle_.joinable()) {
                mainThreadHandle_.join();
            }
        }

        bool accept_thread::start(const options_t& options) {
            bool current = false;

            if (isRunning_.compare_exchange_strong(current, true)) {
                if (!socket_.acceptOn(options.port, options.backlog)) {
                    return false;
                }

                socket_.setTimeout(options.updateIntervall);

                if (mainThreadHandle_.joinable()) {
                    mainThreadHandle_.join();
                }
                
                mainThreadHandle_ = std::thread(&accept_thread::mainThread, this, std::ref(options));
                
                return true;
            }
            
            return false;
        }

        bool accept_thread::stop() {
            bool current = true;
            return isRunning_.compare_exchange_strong(current, false);
        }

        void accept_thread::mainThread(const options_t& options) {
            while (isRunning_.load()) {
                auto connection = socket_.acceptConnection();
                if (connection != nullptr) {
                    std::lock_guard<std::mutex> lg(*options.mutex);

                    options.connectionQueue->push(connection);
                    options.sender->executor.invoke<send_thread::addConnectionTask>();
                }
            }
        }
    }
}

