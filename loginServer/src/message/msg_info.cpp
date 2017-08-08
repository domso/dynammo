#include "network/udp_socket.h"

#include "src/controller/login_server_context.h"

#include "src/util/timestamp.h"

#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/msg_info.h"

namespace message {
    message::msg_status_t msg_info::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        msg_info_response_t* response = outputBuffer.pushNext<msg_info_response_t>();

        if (response != nullptr) {
            response->info.id = server.getID();
            response->info.uptime = util::timestamp::uptime();

            return message::status::ok;
        }

        return message::status::error::unknown;
    }

    msg_status_t msg_info::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::login_server_context& server) {
        // Ignore incoming responses
        return message::status::close;
    }
}


