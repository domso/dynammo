#include "network/udp_socket.h"

#include "include/data/context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_lookup.h"

namespace message {
    const uint8_t msg_lookup::id;

    bool msg_lookup::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        msg_lookup_request_t* request = outputBuffer.pushNext<msg_lookup_request_t>();

        if (request != nullptr) {
            client.getIdentification(request->identification);
            return client.popTicket(request->ticket);
        }

        return false;
    }

    msg_status_t msg_lookup::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        // Ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_lookup::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        msg_lookup_response_t* response;
        response = inputBuffer.getNext<msg_lookup_response_t>();

        if (response != nullptr) {
            if (header.status == MSG_STATUS_OK) {
                client.setValidatedID(response->accountID);
            }
        }
    }
}
