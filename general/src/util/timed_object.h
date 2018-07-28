#pragma once

#include <chrono>
#include <thread>

namespace util {
    template <typename T>
    class timed_object {
    public:  
        timed_object(const T& o) : obj(o) {
            m_birthdate = std::chrono::high_resolution_clock::now();
        }      
        
        void wait_till_age(const int msAge) {
            std::chrono::milliseconds waitTime(msAge);
            std::this_thread::sleep_until(m_birthdate + waitTime);            
        }
        
        auto age() {
            return std::chrono::high_resolution_clock::now() - m_birthdate;
        }
        
        T obj;
    private:              
        std::chrono::time_point<std::chrono::high_resolution_clock> m_birthdate;
    };
}
