#include "network/udp_socket.h"

#include "src/controller/account/login_controller_context.h"

#include "src/util/timestamp.h"

#include "src/authentication/types.h"

#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/account/msg_register.h"

namespace message {
    namespace account {
        bool msg_register::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::account::login_controller_context& server) {
            msg_register_request_t* request = outputBuffer.pushNext<msg_register_request_t>();

            if (request != nullptr) {
                server.getCredentials(request->credentials);
                server.getServerID(request->serverID);
                return true;
            }

            return false;
        }

        msg_status_t msg_register::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& server) {
            // Ignore incoming requests
            return message::status::close;
        }

        msg_status_t msg_register::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& server) {
            server.setRegisterState(header.status == message::status::ok);

            // close message
            return message::status::close;
        }
    }
}


