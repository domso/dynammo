#ifndef gameClient_message_callback_auth_h
#define gameClient_message_callback_auth_h

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/content.h"

#include <iostream>

namespace message {
    class auth {
        public:
            typedef message::content::auth content;
            
            constexpr static const auto id = content::id;
            
            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, content* context) {
                auto request = outputBuffer.push_next<message::content::auth::types::request>();

                if (request != nullptr) {
                    *request = context->request;
                    context->currentState.set(message::types::states::wait);
                    return true;
                }

                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, content* context) {
                return message::status::error::unknown;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, content* context) {
                auto response = inputBuffer.get_next<message::content::auth::types::response>();
                                
                if (response != nullptr) {
                    context->response = *response;          
                }
                                
                context->currentState.set(message::types::states::recv);
                context->status = header.status;
                
                return message::status::close;
            }

        };
}
#endif
