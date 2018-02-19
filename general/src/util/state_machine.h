#ifndef general_util_state_machine_h
#define general_util_state_machine_h

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
        
        void set(const stateT newState) {
            std::lock_guard<std::mutex> lg(m_mutex);
            current = newState;
            m_cond.notify_all();
        }
        
        bool is(const stateT reqState) {
            std::lock_guard<std::mutex> lg(m_mutex);
            return current == reqState;
        }
        
        bool wait_for(const stateT target, const double timeOutSec = 10) {
            std::unique_lock<std::mutex> ul(m_mutex);
                       
            while (current != target) {
                std::chrono::duration<double> duration(timeOutSec);
                if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
                    return false;
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
