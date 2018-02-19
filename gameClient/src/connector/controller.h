#ifndef gameClient_connector_controller_h
#define gameClient_connector_controller_h

#include <thread>
#include <mutex>

#include "src/message/msg_controller.h"
#include "src/connector/tcp_receiver.h"

namespace connector {
    class controller {
    public:
        controller(network::ipv4_addr& tcpDestAddr, network::ipv4_addr& udpDestAddr);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        // call only after all objs are registered!
        bool open();
        
        template <typename T>
        bool exec_request() {
            std::lock_guard<std::mutex> lg(m_mutex);
            return m_msgCtrl.exec_request<T>(m_udpDestAddr, m_outputBuffer);
        }

        template <typename T>
        void register_data_obj(T& obj) {
            m_tcpRecv.register_callbacks<T, T, connector::controller>(&obj, this);
        }
        
        template <typename T>
        void register_message_obj(typename T::content& obj) {
            m_msgCtrl.register_handler<T, typename T::content>(&obj);
        }
        
        template <typename T>
        typename T::content& get_message_obj() {
            return *(m_msgCtrl.additional_datatype<T, typename T::content>());
        }        
    private:
        constexpr static const int bufferSize = 1500;
        network::pkt_buffer m_outputBuffer;
        std::mutex m_mutex;

        bool m_status;

        network::ipv4_addr m_udpDestAddr;
        network::ipv4_addr m_tcpDestAddr;

        std::thread m_msgCtrlThread;
        std::thread m_tcpRecvThread;

        message::msg_controller m_msgCtrl;
        connector::tcp_receiver m_tcpRecv;
    };
}

#endif
