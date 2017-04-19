#include "network/udp_socket.h"

#include "include/data/login_server_context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_auth.h"

namespace message {
    const uint8_t msg_auth::id;

    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::login_server_context& server) {
        msg_auth_request_t* request = inputBuffer.getNext<msg_auth_request_t>();
        msg_auth_response_t* response = outputBuffer.pushNext<msg_auth_response_t>();
        
        if (request != nullptr && response != nullptr) {
            if (server.authenticate(request->credentials, response->ticket)) {              
                response->ticket.userAddr = srcAddr;
                server.getDestination(response->ticket, srcAddr) ;               
                
                 return MSG_STATUS_OK;
             } else {
                 return MSG_STATUS_AUTH_ERROR;
             }
        }

        return MSG_STATUS_UNKOWN_ERROR;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::login_server_context& server) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}


