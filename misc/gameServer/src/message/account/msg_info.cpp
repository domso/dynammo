#include "network/udp_socket.h"
#include "src/controller/account/login_controller_context.h"
#include "src/util/timestamp.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/account/msg_info.h"

namespace message {
    namespace account {
        bool msg_info::request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, controller::account::login_controller_context& client) {
            // just send
            return true;
        }

        msg_status_t msg_info::requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& client) {
            // Ignore incoming requests
            return message::status::close;
        }

        msg_status_t msg_info::responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, controller::account::login_controller_context& client) {
            msg_info_response_t* response;
            response = inputBuffer.get_next<msg_info_response_t>();

            if (response != nullptr) {
                client.setServerInfo(response->info);
            }

            return message::status::close;
        }
    }
}


