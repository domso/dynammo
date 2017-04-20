#ifndef loginServer_message_msg_auth_h
#define loginServer_message_msg_auth_h

#include "network/udp_socket.h"
#include "include/data/login_server_context.h"
#include "include/message/msg_header.h"

namespace message {
    class msg_auth {
    public:
        static const message::msg_type_t id = 1;

        static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void* param);

        static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::login_server_context& server);

        static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::login_server_context& server);
    };
}

#endif
