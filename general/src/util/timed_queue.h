#pragma once

#include <chrono>
#include <queue>
#include <thread>

namespace util {
    template <typename T>
    class timed_queue {
    public:
        struct itemT {
            itemT(const T& arg) : pair(std::chrono::high_resolution_clock::now(), arg){
                
            }
            std::pair<std::chrono::time_point<std::chrono::high_resolution_clock>, T> pair;
        };
        
        bool empty() const {
            return m_queue.empty();
        }
        
        int size() const {
            return m_queue.size();
        }
        
        T& wait_front(const int msDelay) {            
            auto currentTP = std::chrono::high_resolution_clock::now();
            std::chrono::milliseconds age(std::chrono::duration_cast<std::chrono::milliseconds>(currentTP - m_queue.front().pair.first).count());
            std::chrono::milliseconds maxTime(msDelay);
            std::this_thread::sleep_for(maxTime - age);
            
            return m_queue.front().pair.second;
        }
                       
        void pop() {
            m_queue.pop();
        }
        
        void push(const T& arg) {
            m_queue.push(std::make_pair(std::chrono::high_resolution_clock::now(), arg));
        }
        
        void push(const itemT& arg) {
            m_queue.push(arg.pair.second);
        }
    private:
        std::queue<itemT> m_queue;
    };
}
