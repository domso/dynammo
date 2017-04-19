#include "network/udp_socket.h"
#include "include/data/session_context.h"
#include "include/util/timestamp.h"
#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_auth.h"

namespace message {
    const uint8_t msg_auth::id;

    bool msg_auth::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::session_context& client) {
        return false;
    }

    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::session_context& client) {

        // ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::session_context& client) {
        msg_auth_response_t* response = inputBuffer.getNext<msg_auth_response_t>();
        msg_login_request_t* request = outputBuffer.pushNext<msg_login_request_t>();
        
        if (response != nullptr && request != nullptr) {
            client.createNewUser(response->ticket, request->session);
            srcAddr = response->ticket.userAddr;
            
            return MSG_STATUS_OK;
        }

        // close message
        return MSG_STATUS_CLOSE;
    }
}


