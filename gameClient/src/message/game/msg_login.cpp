#include "network/udp_socket.h"
#include "src/controller/account/login_controller_context.h"
#include "src/util/timestamp.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/game/msg_login.h"

namespace message {
    namespace game {
        bool msg_login::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::login_controller_context& client) {
            msg_login_request_t* request = outputBuffer.pushNext<msg_login_request_t>();

            if (request != nullptr) {
                authentication::signed_session_t signed_session;
                client.getSession(signed_session);

                request->session = signed_session.session;

                unsigned char* signature = outputBuffer.pushNext<unsigned char>(signed_session.signature.size());

                if (signature != nullptr) {
                    for (int i = 0; i < signed_session.signature.size(); i++) {
                        signature[i] = signed_session.signature[i];
                    }
                }

                return true;
            }

            return false;
        }

        msg_status_t msg_login::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, message::msg_option_t& options, controller::login_controller_context& client) {
            // Ignore incoming responses
            return message::status::close;
        }

        msg_status_t msg_login::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, message::msg_option_t& options, controller::login_controller_context& client) {
            client.setLoginState(header.status == message::status::ok);

            // no direct response
            return message::status::close;
        }
    }
}


