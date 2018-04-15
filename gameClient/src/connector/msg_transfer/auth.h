#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/types/game_events.h"
#include "src/connector/msg_transfer/data.h"
#include "src/connector/msg_transfer/sign_buffer.h"

namespace connector {
    namespace msg_transfer {
        class auth {
        public:
            typedef types::msg_transfer::content::auth content;

            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, types::game_events event, connector::msg_transfer::data* data) {
                auto request = outputBuffer.push_next<content::types::request>();            
                
                if (request != nullptr) {
                    request->accountID = data->sessionCtrl.get_accountID();
                    request->tcpTicket = data->sessionCtrl.get_tcp_link();
                                        
                    return sign_buffer(outputBuffer, data->sessionCtrl);
                }

                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {
                return message::status::error::unknown;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {
                auto response = inputBuffer.get_next<content::types::response>();

                if (response != nullptr) {
//                     context->response = *response;
                }
                
                return message::status::close;
            }
        };
    }
}
