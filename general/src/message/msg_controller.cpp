#include "src/message/msg_controller.h"

message::msg_controller::msg_controller(const int bufferSize) :
    m_inputBuffer(bufferSize), m_outputBuffer(bufferSize) {

    for (int i = 0; i < 256; i++) {
        m_callbacks[i] = nullptr;
    }
}

bool message::msg_controller::init(const uint16_t port) {
    return m_networkSocket.init(port);
}

void message::msg_controller::close() {
    m_networkSocket.shut_RDWR();
}

void message::msg_controller::operator()() {
    recv();
}

void message::msg_controller::recv() {
    static_assert(sizeof(msg_header_t::msgType) == 1, "bad header-type");
    assert(m_additional_data != nullptr);
    msg_header_t* header;

    while (internal_recv()) {
        header = m_inputBuffer.get_next<msg_header_t>();

        if (header != nullptr) {
            if (m_callbacks[header->msgType] != nullptr) {
                m_outputBuffer.set_msg_length(0);

                message::msg_option_t option = message::option::clear;
                message::msg_header_t* outputHeader = m_outputBuffer.push_next<message::msg_header_t>();

                if (outputHeader != nullptr) {
                    outputHeader->status = message::status::ok;
                    outputHeader->msgType = header->msgType ^ (message::request_switch_mask * ((option & message::option::no_request_response_switch) == 0));
                    outputHeader->attr = header->attr;

                    outputHeader->status = m_callbacks[header->msgType](*header, m_srcAddr, m_inputBuffer, m_outputBuffer, m_networkSocket, option, m_additional_data[header->msgType]);

                    outputHeader->msgType = header->msgType ^ (message::request_switch_mask * ((option & message::option::no_request_response_switch) == 0));
                    outputHeader->attr = header->attr;

                    m_inputBuffer.set_msg_length(0);
                    if (outputHeader->status != message::status::close) {
                        internal_send(m_networkSocket, m_srcAddr,  m_outputBuffer);
                    }                    
                }
            }
        } else {
            m_inputBuffer.set_msg_length(0);
        }
    }
}

uint16_t message::msg_controller::port() const {
    return m_networkSocket.get_addr().port();
}

bool message::msg_controller::internal_recv() {
    if (m_inputBuffer.msg_length() != 0) {
        return true;
    }

    return m_networkSocket.recv_pkt(m_srcAddr, m_inputBuffer).first;
}

bool message::msg_controller::internal_send(network::udp_socket<network::ipv4_addr>& networkSocket, network::ipv4_addr& dest, network::pkt_buffer& outputBuffer, const int flags) {
    return networkSocket.send_pkt(dest, outputBuffer, flags).first;
}




