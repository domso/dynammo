#ifndef gameClient_message_callback_info_h
#define gameClient_message_callback_info_h

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/content.h"
#include "src/connector/context.h"

namespace message {
    namespace callback {
        class info {
        public:
            constexpr static const auto id = message::content::info::id;
            
            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<message::content::info::types::request>();
                auto response = outputBuffer.push_next<message::content::info::types::response>();
                auto result = message::status::error::unknown;

                if (request != nullptr && response != nullptr) {
                    response->regID = request->regID;
//                     response->port = context->get_region_port(request->regID);

                    result = message::status::ok;
                }

                return result;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                return message::status::error::unknown;
            }
        };
    }
}
#endif
