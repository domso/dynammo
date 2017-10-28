#include "network/udp_socket.h"
#include "src/util/timestamp.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/game/msg_login.h"
#include <src/encryption/rsa.h>

namespace message {
    namespace game {
        bool msg_login::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::game::user_controller_context& server) {
            return false;
        }

        msg_status_t msg_login::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server) {
            msg_login_request_t* request = inputBuffer.getNext<msg_login_request_t>();
            message::msg_type_t result = message::status::error::unknown;

            
            std::cout << "received request for login!" << std::endl;
            if (request != nullptr) {
                encryption::signature signature(inputBuffer.getNext<uint8_t>(inputBuffer.remainingMsgLen()), inputBuffer.remainingMsgLen());
                
                if (server.login(request->ticket, request->session, signature)) {
                    result = message::status::ok;
                } else {
                    result = message::status::error::access_denied;
                }
            }

            return result;
        }

        msg_status_t msg_login::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::game::user_controller_context& server) {
            // ignore incoming responses
            return message::status::close;
        }
    }
}



