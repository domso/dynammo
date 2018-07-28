#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/types/game_events.h"
#include "src/connector/context.h"
#include "src/connector/sign_buffer.h"

namespace connector {
    namespace msg_transfer {
        class region_action {
        public:
            typedef types::msg_transfer::content::region_action content;

            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, uint64_t eventArg, connector::msg_transfer::context* context) {
                auto request = outputBuffer.push_next<content::types::request>();

                if (request != nullptr) {
//                     request->sessionID = data->sessionCtrl.get_tcp_link();
                    request->actionID = (::types::game_events)eventArg;
                    request->regionID = 0;
//                     return sign_buffer(outputBuffer, data->sessionCtrl);
                }

                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::context* context) {
                return message::status::error::unknown;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::context* context) {               
                return message::status::close;
            }
        };
    }
}



