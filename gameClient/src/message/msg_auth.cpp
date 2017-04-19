#include "network/udp_socket.h"
#include "include/data/login_context.h"
#include "include/util/timestamp.h"
#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_auth.h"

namespace message {
    const uint8_t msg_auth::id;

    bool msg_auth::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::login_context& client) {
        msg_auth_request_t* request = outputBuffer.pushNext<msg_auth_request_t>();
        
        if (request != nullptr) {
            client.getCredentials(request->credentials);
            return true;
        }
        
        return false;
    }

    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::login_context& client) {
        msg_login_request_t* request = inputBuffer.getNext<msg_login_request_t>();
        
        if (request != nullptr) {
            client.setSession(request->session);
        }
        
        // no direct response
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket< network::ipv4_addr >& socket, data::login_context& client) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}


