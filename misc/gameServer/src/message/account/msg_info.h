#ifndef gameServer_message_account_msg_info_h
#define gameServer_message_account_msg_info_h

#include "network/udp_socket.h"
#include "src/controller/account/login_controller_context.h"
#include "src/message/msg_header.h"

namespace message {
    namespace account {
        class msg_info {
        public:
            constexpr static const message::msg_type_t id = 0;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::account::login_controller_context& client);

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& client);

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& client);

        };
    }
}

#endif
