#include "network/udp_socket.h"
#include "include/data/context.h"
#include "include/util/timestamp.h"
#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_info.h"

namespace message {
    const uint8_t msg_info::id;

    bool msg_info::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        // just send
        return true;
    }

    msg_status_t msg_info::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        // Ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_info::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& client) {
        msg_info_response_t* response;
        response = inputBuffer.getNext<msg_info_response_t>();

        if (response != nullptr) {
            client.setServerInfo(response->info);
        }
    }
}


