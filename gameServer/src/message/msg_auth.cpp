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

    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& client) {

        // ignore incoming requests
        return MSG_STATUS_CLOSE;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, data::session_context& client) {
        msg_auth_response_t* response = inputBuffer.getNext<msg_auth_response_t>();
        msg_session_response_t* redirectedResponse = outputBuffer.pushNext<msg_session_response_t>();
        
        if (response != nullptr && redirectedResponse != nullptr) {
            client.createNewUser(response->ticket, redirectedResponse->session);
            redirectedResponse->addr = response->ticket.userAddr; 
            
            options &= MSG_OPTION_NO_REQUEST_RESPONSE_SWITCH;
            return MSG_STATUS_OK;
        }

        // close message
        return MSG_STATUS_CLOSE;
    }
}


