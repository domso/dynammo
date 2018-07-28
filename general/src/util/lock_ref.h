#ifndef general_util_lock_ref_h
#define general_util_lock_ref_h

#include <mutex>
#include <cassert>

namespace util {
    class locked_ref_item {
    public:
        locked_ref_item() {
            
        }
        
        locked_ref_item(locked_ref_item& copy) {
            std::lock_guard<std::mutex> lg(copy.m_mutex);
        }
        
        locked_ref_item(locked_ref_item&& move) {
            std::lock_guard<std::mutex> lg(move.m_mutex);
        }
        
        ~locked_ref_item() {
            std::lock_guard<std::mutex> lg(m_mutex);
        }
        
        void operator=(locked_ref_item& copy) {
            std::lock_guard<std::mutex> lg(copy.m_mutex);
        }
        
        void operator=(locked_ref_item&& move) {
            std::lock_guard<std::mutex> lg(move.m_mutex);
        }
        
        std::mutex& mutex() {
            return m_mutex;
        }
    private:
        std::mutex m_mutex;
    };


    // T must implement locked_ref_item
    template <typename T>
    class locked_ref {
    public:
        locked_ref(T& data) : m_valid(true), m_data(data), m_lock(data.mutex()) {}
        locked_ref(const locked_ref& copy) = delete;
        locked_ref(locked_ref&& move) : m_data(move.m_data), m_lock(std::move(move.m_lock)) {}

        T* operator->() {
            assert(m_valid == true);
            return &m_data;
        }
        
        T* data() {
            assert(m_valid == true);
            return &m_data;
        }
        
        void release() {
            m_valid = false;
            m_lock.unlock();
        }
    private:
        bool m_valid;
        T& m_data;
        std::unique_lock<std::mutex> m_lock;
    };
}

#endif
