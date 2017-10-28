#include "network/udp_socket.h"

#include "src/controller/login_server_context.h"

#include "src/util/timestamp.h"

#include "src/authentication/types.h"

#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/msg_auth.h"

namespace message {
    msg_status_t msg_auth::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        msg_auth_request_t* request = inputBuffer.getNext<msg_auth_request_t>();
        msg_auth_response_t* response = outputBuffer.pushNext<msg_auth_response_t>();
        message::msg_status_t status = message::status::error::unknown;

        if (request != nullptr && response != nullptr) {
            status = server.authenticate(request->credentials, response->session);
            if (status == message::status::ok) {
                status = server.getDestination(response->session, response->session.serverAddr);
            }
        }

        if (server.getPrivateKey() != nullptr && status == message::status::ok) {
            int requiredSize = server.getPrivateKey()->getRequiredSize();
            encryption::signature signature(outputBuffer.pushNext<unsigned char>(requiredSize), requiredSize);

            if (signature.data == nullptr || encryption::sign<authentication::session_t>(*server.getPrivateKey(), signature, &response->session) == 0) {
                status = message::status::error::signing;
            }
        }

        return status;
    }

    msg_status_t msg_auth::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        return message::status::close;
    }
}


