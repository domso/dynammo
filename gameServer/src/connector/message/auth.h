#ifndef gameClient_message_callback_auth_h
#define gameClient_message_callback_auth_h

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/message/content.h"
#include "src/connector/context.h"

#include <iostream>

namespace message {
    namespace callback {
        class auth {
        public:
            constexpr static const auto id = message::content::auth::id;
            
            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<message::content::auth::types::request>();
                auto response = outputBuffer.push_next<message::content::auth::types::response>();
                auto result = message::status::error::unknown;
                
                if (request != nullptr && response != nullptr) {
                    response->accountID = request->accountID;
                    {
                        auto info = context->m_userCtrl.get_info(request->accountID);
                        info->connection = context->m_connectionCtrl.get_new_connection(request->tcpTicket);
                    }
                    {
                        auto region = context->m_regionCtrl.get_region(0);
                        region->add_user(request->accountID);
                    }
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
