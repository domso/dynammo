#pragma once

#include <type_traits>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <functional>

namespace util {
    template <typename eventT, typename argT = uint64_t>
    class event_controller {
    public:
        event_controller(const eventT clearEvent) : m_clearEvent(clearEvent) {
            
        }
                
        void new_event(const eventT newEvent, const argT arg = argT()) {
            std::lock_guard<std::mutex> lg(m_mutex);
            std::vector<eventT> eventStack;
            eventStack.push_back(newEvent);
            
            while (!eventStack.empty()) {
                eventT currentEvent = eventStack.back();
                auto callbacks = m_callbacks.find(currentEvent);
                eventStack.pop_back();
                if(callbacks != m_callbacks.end()) {
                    for (auto& c : callbacks->second) {
                        eventStack.push_back(c.first(currentEvent, arg, c.second));
                    }
                }
            }
        }
        
        template <typename T, typename additionalT>
        void register_event_handler(additionalT* param) {
            std::lock_guard<std::mutex> lg(m_mutex);
            eventT(*typeGuard)(const eventT, const argT&, additionalT*) = &T::handle;
            m_callbacks[T::trigger].push_back(std::make_pair((eventT(*)(const eventT, const argT&, void*)) typeGuard, (void*)param));
        }   
        
        template <typename T>
        void unregister_event_handler() {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_callbacks.erase(T::trigger);
        }              
    private:
        eventT m_clearEvent;
        std::mutex m_mutex;        
        std::unordered_map<eventT, std::vector<std::pair<std::function<eventT(const eventT, const argT&, void*)>, void*>>> m_callbacks;
    };
}
