#ifndef gameServer_message_msg_unregister_h
#define gameServer_message_msg_unregister_h

#include "network/udp_socket.h"
#include "include/data/session_context.h"
#include "include/message/msg_header.h"

namespace message {
    class msg_unregister {
    public:
        static const message::msg_type_t id = 3;

        static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::session_context& server);

        static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& server);

        static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& server);
    };
}

#endif
