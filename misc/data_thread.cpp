#include "src/data/map/data_thread.h"

namespace data {
    namespace map {
        data_thread::data_thread() {

        }

        data_thread::~data_thread() {
            if (m_mainThreadHandle.joinable()) {
                m_mainThreadHandle.join();
            }
        }

        bool data_thread::start(const options_t& options) {
            bool current = false;

            if (m_isRunning.compare_exchange_strong(current, true)) {
                m_mainThreadHandle = std::thread(&data_thread::mainThread, this, std::ref(options));
                return true;
            }

            return false;
        }

        bool data_thread::stop() {
            bool current = true;
            return m_isRunning.compare_exchange_strong(current, false);
        }

        void data_thread::mainThread(const options_t& options) {
            while (m_isRunning.load()) {
                //TODO
            }
        }
    }
}


