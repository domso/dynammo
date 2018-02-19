#include "network/udp_socket.h"
#include "src/controller/account/login_controller_context.h"
#include "src/util/timestamp.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/account/msg_auth.h"

namespace message {
    namespace account {
        bool msg_auth::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::login_controller_context& client) {
            msg_auth_request_t* request = outputBuffer.push_next<msg_auth_request_t>();

            if (request != nullptr) {
//                 client.getCredentials(request->credentials);
                return true;
            }

            return false;
        }

        msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, message::msg_option_t& options, controller::login_controller_context& client) {
            // Ignore incoming responses
            return message::status::close;
        }

        msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, message::msg_option_t& options, controller::login_controller_context& client) {
            msg_auth_response_t* response = inputBuffer.get_next<msg_auth_response_t>();
            int sigLen = inputBuffer.remaining_msg_length();
            encryption::signature signature(inputBuffer.get_next<unsigned char>(sigLen), sigLen);

            if (response != nullptr && signature.data != nullptr) {
//                 client.setSession(response->session, signature);

            } else {
                //TODO add error
            }

            // close message
            return message::status::close;
        }
    }
}


