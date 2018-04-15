#pragma once

#include <type_traits>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <functional>

namespace util {
    template <typename eventT>
    class event_controller {
    public:
        event_controller(const eventT clearEvent) : m_clearEvent(clearEvent) {
            
        }
                
        eventT new_event(const eventT newEvent) {
            std::lock_guard<std::mutex> lg(m_mutex);
            std::vector<eventT> eventStack;
            eventStack.push_back(newEvent);
            
            while (!eventStack.empty()) {
                eventT currentEvent = eventStack.back();
                auto callbacks = m_callbacks.find(currentEvent);
                eventStack.pop_back();
                if(callbacks != m_callbacks.end()) {
                    for (auto& c : callbacks->second) {
                        eventStack.push_back(c.first(currentEvent, c.second));
                    }
                }
            }
        }
        
        template <typename T, typename additionalT>
        void register_event_handler(additionalT* param) {
            std::lock_guard<std::mutex> lg(m_mutex);
            eventT(*typeGuard)(eventT ID, additionalT*) = &T::handle;
            m_callbacks[T::trigger].push_back(std::make_pair((eventT(*)(eventT ID, void*)) typeGuard, (void*)param));
        }   
        
        template <typename T>
        void unregister_event_handler() {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_callbacks.erase(T::trigger);
        }       
        
    private:
        eventT m_clearEvent;
        std::mutex m_mutex;        
        std::unordered_map<eventT, std::vector<std::pair<std::function<eventT(eventT ID, void*)>, void*>>> m_callbacks;
    };
}
