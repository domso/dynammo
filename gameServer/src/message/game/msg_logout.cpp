#include "network/udp_socket.h"
#include "src/util/timestamp.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/game/msg_logout.h"

namespace message {
    namespace game {
        bool msg_logout::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::game::user_controller_context& server) {
            return false;
        }

        msg_status_t msg_logout::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server) {
            msg_logout_request_t* request = inputBuffer.getNext<msg_logout_request_t>();
            message::msg_type_t result = message::status::error::unknown;
            
            if (request != nullptr) {
                if (server.logout(request->session)) {
                    result = message::status::ok;
                } else {
                    result = message::status::error::permission;
                }
            }

            return result;
        }

        msg_status_t msg_logout::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server) {
            // ignore incoming responses
            return message::status::close;
        }
    }
}


