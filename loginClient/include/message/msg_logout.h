#ifndef loginClient_message_msg_logout_h
#define loginClient_message_msg_logout_h

#include "network/udp_socket.h"
#include "include/data/context.h"
#include "include/message/msg_header.h"

namespace message {
    class msg_logout {
    public:
        static const message::msg_type_t id = 2;

        static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::context& client);

        static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::context& client);

        static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::context& client);
    };
}

#endif
