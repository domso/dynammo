#include "network/udp_socket.h"

#include "include/data/login_server_context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_register.h"

#include <iostream>
namespace message {
    const uint8_t msg_register::id;

    msg_status_t msg_register::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::login_server_context& server) {
        msg_register_request_t* request = inputBuffer.getNext<msg_register_request_t>();
        if (request != nullptr) {
            if (server.registerService(request->credentials, request->serverID, srcAddr)) {
                 return MSG_STATUS_OK;
             } else {
                 return MSG_STATUS_AUTH_ERROR;
             }
        }

        return MSG_STATUS_UNKOWN_ERROR;
    }

    msg_status_t msg_register::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::login_server_context& server) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}


