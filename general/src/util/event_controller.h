#pragma once

#include <type_traits>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <functional>
#include <optional>

namespace util {
    template <typename eventT>
    class event_controller {
    public:
        event_controller() {

        }

        void new_event(const eventT newEvent) {
            auto handler = get_handler(newEvent);
            
            if (handler) {
                (*handler).first(newEvent, (*handler).second);
            }
        }

        template <typename additionalT>
        void register_event_handler(const eventT trigger, void(*typeGuard)(const eventT, additionalT*), additionalT* param) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_callbacks[trigger] = std::make_pair((void(*)(const eventT, void*)) typeGuard, (void*)param);
        }

        void unregister_event_handler(const eventT trigger) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_callbacks.erase(trigger);
        }
    private:
        std::optional<std::pair<std::function<void(const eventT, void*)>, void*>> get_handler(const eventT newEvent) {
            std::lock_guard<std::mutex> lg(m_mutex);
            
            auto it = m_callbacks.find(newEvent);
            
            if (it != m_callbacks.end()) {
                return it->second;
            }

            return std::nullopt;
        }
        std::mutex m_mutex;
        std::unordered_map<eventT, std::pair<std::function<void(const eventT, void*)>, void*>> m_callbacks;
    };
}
