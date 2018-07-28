#pragma once

#include <thread>

namespace util {
    template <typename T>
    class worker {
    public:
        worker(T& obj) : m_obj(obj) {
            m_thread = std::thread(&T::update, &obj);
        }
        worker(const worker&) = delete;
        worker(worker&&) = delete;
        ~worker() {
            m_obj.close();
            m_thread.join();
        } 
    private:
        T& m_obj;
        std::thread m_thread;
    };
}
