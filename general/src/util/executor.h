#ifndef util_executor_h
#define util_executor_h

#include <stdint.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <atomic>
#include "network/pkt_buffer.h"

namespace util {
    template <typename contextT, typename contextArgT = int>
    class executor {
    public:

        executor() : m_isRunning(false), m_callbacks(256) {

        }

        ~executor() {
            stop();

            if (m_threadHandle.joinable()) {
                m_threadHandle.join();
            }
        }

        executor(const executor&) = delete;

        template <typename taskT>
        bool registerTask() {
            static_assert(sizeof(taskT::id) == 1);

            if (m_callbacks[taskT::id] == nullptr) {
                m_callbacks[taskT::id] = taskT::execute;
                return true;
            }

            return false;
        }

        template <typename taskT>
        bool invoke() {
            if (!m_isRunning.load()) {
                return false;
            }

            {
                std::unique_lock<std::mutex> ul(m_taskMutex);
                int8_t task = taskT::id;
                m_tasks.push(task);
                m_taskCond.notify_one();
            }

            return true;
        }

        bool start() {
            bool tmp = false;

            if (m_isRunning.compare_exchange_strong(tmp, true)) {
                if (m_threadHandle.joinable()) {
                    m_threadHandle.join();
                }

                m_threadHandle = std::thread(&executor::threadMainNoArg, this);
                return true;
            }

            return false;
        }

        bool start(const contextArgT& arg) {
            bool tmp = false;

            if (m_isRunning.compare_exchange_strong(tmp, true)) {
                if (m_threadHandle.joinable()) {
                    m_threadHandle.join();
                }

                m_threadHandle = std::thread(&executor::threadMainArg, this, std::ref(arg));

                return true;
            }

            return false;
        }

        bool stop() {
            bool tmp = true;
            return m_isRunning.compare_exchange_strong(tmp, false);
        }

    private:

        void threadMainNoArg() {
            contextT context;
            threadMainWork(context);
        }

        void threadMainArg(const contextArgT& arg) {
            contextT context(arg);
            threadMainWork(context);
        }

        void threadMainWork(contextT& context) {
            while (true) {
                std::unique_lock<std::mutex> ul(m_taskMutex);

                while (m_tasks.empty() && m_isRunning.load()) {
                    m_taskCond.wait(ul);
                }

                while (!m_tasks.empty()) {
                    int8_t task = m_tasks.front();
                    m_tasks.pop();

                    ul.unlock();
                    m_callbacks[task](context);
                    ul.lock();
                }

                if (!m_isRunning.load()) {
                    return;
                }
            }
        }

        std::thread m_threadHandle;
        std::atomic<bool> m_isRunning;
        std::mutex m_taskMutex;
        std::condition_variable m_taskCond;
        std::queue<int8_t> m_tasks;
        std::vector<std::function<void(contextT&)>> m_callbacks;
    };
}

#endif
