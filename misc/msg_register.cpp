#include "network/udp_socket.h"

#include "src/controller/login_server_context.h"

#include "src/util/timestamp.h"

#include "src/authentication/types.h"

#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/msg_register.h"

namespace message {
    msg_status_t msg_register::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        msg_register_request_t* request = inputBuffer.getNext<msg_register_request_t>();

        if (request != nullptr) {
            return server.registerService(request->credentials, request->serverID, srcAddr);
        }

        return message::status::error::unknown;
    }

    msg_status_t msg_register::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        // Ignore incoming responses
        return message::status::close;
    }
}


