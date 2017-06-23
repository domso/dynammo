#include "network/udp_socket.h"

#include "include/data/session_context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_register.h"

namespace message {
    const uint8_t msg_register::id;

    bool msg_register::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::session_context& server) {
        msg_register_request_t* request = outputBuffer.pushNext<msg_register_request_t>();

        if (request != nullptr) {
            server.getCredentials(request->credentials);
            server.getServerID(request->serverID);
            return true;
        }

        return false;
    }

    msg_status_t msg_register::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& server) {
        // Ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_register::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& server) {
        server.setRegisterState(header.status == MSG_STATUS_OK);

        // close message
        return MSG_STATUS_CLOSE;
    }
}


