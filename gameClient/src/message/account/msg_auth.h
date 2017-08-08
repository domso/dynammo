#ifndef gameClient_message_account_msg_auth_h
#define gameClient_message_account_msg_auth_h

#include "network/udp_socket.h"
#include "src/controller/account/login_controller_context.h"
#include "src/message/msg_header.h"

namespace message {
    namespace account {
        class msg_auth {
        public:
            constexpr static const message::msg_type_t id = 1;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::login_controller_context& client);

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_controller_context& client);

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_controller_context& client);
        };
    }
}

#endif
