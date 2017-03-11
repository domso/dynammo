#include "network/udp_socket.h"

#include "include/data/context.h"

#include "include/util/timestamp.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_info.h"

namespace message {
    const uint8_t msg_info::id;

    message::msg_status_t msg_info::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& server) {
        msg_info_response_t* response = outputBuffer.pushNext<msg_info_response_t>();


        if (response != nullptr) {
            response->info.id = server.getID();
            response->info.numClients = server.getNumClients();
            response->info.uptime = util::timestamp::uptime();

            return MSG_STATUS_OK;
        }

        return MSG_STATUS_UNKOWN_ERROR;
    }

    msg_status_t msg_info::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::context& server) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}


