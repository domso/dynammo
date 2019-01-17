#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <iostream>
namespace util {
    template <typename T, int timeout>
    class block_queue {
    public:
        void push(const T& d) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_queue.push(d);
            m_cond.notify_one();
        }
        std::optional<T> pop() {
            std::unique_lock<std::mutex> ul(m_mutex);

            while (m_queue.empty()) {
                std::chrono::duration<int> duration(timeout);

                if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
                    return std::nullopt;
                }
            }
            
            T result = m_queue.front();
            m_queue.pop();
            return result;
        }
    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_cond;
    };
}
