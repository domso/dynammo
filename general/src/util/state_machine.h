#ifndef general_util_state_machine_h
#define general_util_state_machine_h

#include <memory>
#include <mutex>
#include <condition_variable>

namespace util {
    
    template <typename stateT>
    class state_machine {
    public:
        state_machine() {}
        state_machine(const stateT newState) {
            set(newState);
        }
        
        void operator=(const stateT newState) {
            set(newState);
        }
        
        void operator==(const stateT reqState) {
            return is(reqState);
        }
        
        stateT get() {
            std::lock_guard<std::mutex> lg(m_mutex);
            return current;
        }
        
        void set(const stateT newState) {
            std::lock_guard<std::mutex> lg(m_mutex);
            current = newState;
            m_cond.notify_all();
        }
        
        bool set_from(const stateT oldState, const stateT newState) {
            std::lock_guard<std::mutex> lg(m_mutex);
            bool result = current.compare_exchange_strong(oldState, newState, std::memory_order_release);
            m_cond.notify_all();
            return result;
        }
        
        bool is(const stateT reqState) {
            std::lock_guard<std::mutex> lg(m_mutex);
            return current == reqState;
        }
        
        bool wait_for(const stateT target, const double timeOutSec = -1) {
            std::unique_lock<std::mutex> ul(m_mutex);
                       
            while (current != target) {
                if (timeOutSec > 0) {
                    std::chrono::duration<double> duration(timeOutSec);
                    if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
                        return false;
                    }
                } else {
                    m_cond.wait(ul);
                }
            }
            
            return true;
        }
        
        bool wait_for_not(const stateT target, const double timeOutSec = -1) {
            std::unique_lock<std::mutex> ul(m_mutex);
                       
            while (current == target) {
                if (timeOutSec > 0) {
                    std::chrono::duration<double> duration(timeOutSec);
                    if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
                        return false;
                    }
                } else {
                    m_cond.wait(ul);
                }
            }
            
            return true;
        }
    private:
        stateT current;
        std::mutex m_mutex;
        std::condition_variable m_cond;
    };    
}

#endif
