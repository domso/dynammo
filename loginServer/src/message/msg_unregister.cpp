#include "network/udp_socket.h"

#include "include/data/login_server_context.h"

#include "include/util/timestamp.h"

#include "include/authentication/types.h"

#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/message/msg_unregister.h"

namespace message {
    const uint8_t msg_unregister::id;

    msg_status_t msg_unregister::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::login_server_context& server) {
        msg_unregister_request_t* request = inputBuffer.getNext<msg_unregister_request_t>();
        
        if (request != nullptr) {
            if (server.unregisterService(request->credentials, request->serverID)) {              
                
                 return MSG_STATUS_OK;
             } else {
                 return MSG_STATUS_AUTH_ERROR;
             }
        }

        return MSG_STATUS_UNKOWN_ERROR;
    }

    msg_status_t msg_unregister::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, data::login_server_context& server) {
        // Ignore incoming responses
        return MSG_STATUS_CLOSE;
    }
}


