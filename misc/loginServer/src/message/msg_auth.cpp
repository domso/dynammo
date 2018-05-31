#include "network/udp_socket.h"

#include "src/controller/login_server_context.h"

#include "src/util/timestamp.h"

#include "src/authentication/types.h"

#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/msg_auth.h"

namespace message {
    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        msg_auth_request_t* request = inputBuffer.get_next<msg_auth_request_t>();
        msg_auth_response_t* response = outputBuffer.push_next<msg_auth_response_t>();
        message::msg_status_t status = message::status::error::unavailable;

        if (request != nullptr && response != nullptr) {
            std::string result = server.map_id_to_key(request->accountID);
            
            if (outputBuffer.push_string(result)) {
                response->accountID = request->accountID;
                status = message::status::ok;
            }
        }

        return status;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        return message::status::close;
    }
}


