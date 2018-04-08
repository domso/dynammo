#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/types/game_events.h"
#include "src/session/controller.h"
#include "src/connector/msg_transfer/sign_buffer.h"

namespace connector {
    namespace msg_transfer {
        class action {
        public:
            typedef types::msg_transfer::content::action content;

            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, types::game_events event, session::controller* sessionCtrl) {
                auto request = outputBuffer.push_next<content::types::request>();

                if (request != nullptr) {
                    request->accountID = 0;
                    request->actionID = event;
                    return sign_buffer(outputBuffer, *sessionCtrl);
                }

                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, session::controller* sessionCtrl) {
                return message::status::error::unknown;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, session::controller* sessionCtrl) {               
                return message::status::close;
            }
        };
    }
}

