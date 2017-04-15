#include "network/udp_socket.h"

#include "include/data/context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_lookup.h"

namespace message {
    const uint8_t msg_lookup::id;

    msg_status_t msg_lookup::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& server) {
        msg_lookup_request_t* request = inputBuffer.getNext<msg_lookup_request_t>();
        msg_lookup_response_t* response = outputBuffer.pushNext<msg_lookup_response_t>();

        if (request != nullptr && response != nullptr) {
            if (server.validateTicket(request->identification, request->ticket)) {
                response->ticket = request->ticket;
                return MSG_STATUS_OK;
            } else {
                return MSG_STATUS_AUTH_ERROR;
            }
        }

        return MSG_STATUS_UNKOWN_ERROR;
    }

    msg_status_t msg_lookup::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& server) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}
