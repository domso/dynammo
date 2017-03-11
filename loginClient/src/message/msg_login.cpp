#include "network/udp_socket.h"

#include "include/data/context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_login.h"

namespace message {
    const uint8_t msg_login::id;

    bool msg_login::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        msg_login_request_t* request = outputBuffer.pushNext<msg_login_request_t>();

        if (request != nullptr) {
            client.getCredentials(request->credentials);
            return true;
        }

        return false;
    }

    msg_status_t msg_login::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        // Ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_login::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        msg_login_response_t* response;
        response = inputBuffer.getNext<msg_login_response_t>();

        if (response != nullptr) {
            client.setTicket(response->ticket);
        }
    }
}
