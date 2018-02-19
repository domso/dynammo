#ifndef gameServer_message_game_msg_login_h
#define gameServer_message_game_msg_login_h

#include "network/udp_socket.h"
#include "src/controller/game/user_controller_context.h"
#include "src/message/msg_header.h"

namespace message {
    namespace game {
        class msg_login {
        public:
            constexpr static const message::msg_type_t id = 0;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::game::user_controller_context& server);

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server);

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server);
        };
    }
}

#endif
