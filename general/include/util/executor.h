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

        executor() : isRunning_(false), callbacks_(256) {

        }

        ~executor() {
            stop();

            if (threadHandle_.joinable()) {
                threadHandle_.join();
            }
        }

        executor(const executor&) = delete;

        template <typename taskT>
        bool registerTask() {
            static_assert(sizeof(taskT::id) == 1);

            if (callbacks_[taskT::id] == nullptr) {
                callbacks_[taskT::id] = taskT::execute;
                return true;
            }

            return false;
        }

        template <typename taskT>
        bool invoke() {
            if (!isRunning_.load()) {
                return false;
            }

            {
                std::unique_lock<std::mutex> ul(taskMutex_);
                int8_t task = taskT::id;
                tasks_.push(task);
                taskCond_.notify_one();
            }

            return true;
        }

        bool start() {
            bool tmp = false;

            if (isRunning_.compare_exchange_strong(tmp, true)) {
                if (threadHandle_.joinable()) {
                    threadHandle_.join();
                }

                threadHandle_ = std::thread(&executor::threadMainNoArg, this);
                return true;
            }

            return false;
        }

        bool start(const contextArgT& arg) {
            bool tmp = false;

            if (isRunning_.compare_exchange_strong(tmp, true)) {
                if (threadHandle_.joinable()) {
                    threadHandle_.join();
                }

                threadHandle_ = std::thread(&executor::threadMainArg, this, std::ref(arg));

                return true;
            }

            return false;
        }

        bool stop() {
            bool tmp = true;
            return isRunning_.compare_exchange_strong(tmp, false);
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
                std::unique_lock<std::mutex> ul(taskMutex_);

                while (tasks_.empty() && isRunning_.load()) {
                    taskCond_.wait(ul);
                }

                while (!tasks_.empty()) {
                    int8_t task = tasks_.front();
                    tasks_.pop();

                    ul.unlock();
                    callbacks_[task](context);
                    ul.lock();
                }

                if (!isRunning_.load()) {
                    return;
                }
            }
        }

        std::thread threadHandle_;
        std::atomic<bool> isRunning_;
        std::mutex taskMutex_;
        std::condition_variable taskCond_;
        std::queue<int8_t> tasks_;
        std::vector<std::function<void(contextT&)>> callbacks_;
    };
}

#endif
