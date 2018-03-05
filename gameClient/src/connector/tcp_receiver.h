#ifndef gameClient_connector_tcp_receiver_h
#define gameClient_connector_tcp_receiver_h

#include <mutex>
#include <functional>

#include "network/pkt_buffer.h"
#include "network/tcp_connection.h"

namespace connector {
    class tcp_receiver {
    public:
        tcp_receiver(const int bufferSize);
        tcp_receiver(const tcp_receiver& copy) = delete;
        tcp_receiver(tcp_receiver&& copy) = delete;
        ~tcp_receiver();

        bool init(const network::ipv4_addr& addr);

        class target {
        public:            
            target();
            target(const target& copy);
            target(target&& move);
            void operator=(const target& copy);
            void operator=(target&& move);
            
            template <typename T>
            void set(T* dest, const int n = 1) {
                static_assert(std::is_trivially_copyable<T>::value);
                m_destPtr = (uint8_t*)dest;
                m_destSize = sizeof(T) * n;
            }
            
            uint8_t* destination();
            int size() const;            
            bool complete() const; 
            void reset();
        private:
            uint8_t* m_destPtr;
            int m_destSize;
        };

        template <typename T>
        void register_callbacks(T* param) {
            static_assert(sizeof(T::id) == 1, "");

            target (*configureTypeGuard)(T*) = &T::configure;
            bool (*completeTypeGuard)(T*) = &T::complete;

            m_configureCallback[T::id] = (target(*)(void*)) configureTypeGuard;
            m_completeCallback[T::id] = (bool(*)(void*)) completeTypeGuard;
            m_callbackDataArg[T::id] = (void*) param;
        }

        void close();
        void recv();
    private:
        bool internal_recv();
        void call_configure(std::unique_lock<std::mutex>& ul);
        void call_complete(std::unique_lock<std::mutex>& ul);
        void copy_bytes();

        std::mutex m_mutex;
        target m_currentTarget;
        uint8_t m_currentMode;
        bool m_modeIsValid;
        
        std::function<target(void*)> m_configureCallback[256];
        std::function<bool(void*)> m_completeCallback[256];
        void* m_callbackDataArg[256];
        
        network::tcp_connection<network::ipv4_addr> m_connection;
        network::pkt_buffer m_inputBuffer;
    };
}

#endif
