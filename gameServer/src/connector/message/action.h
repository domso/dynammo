#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/types/game_events.h"

namespace message {
    namespace callback {
        class action {
        public:
            typedef ::types::msg_transfer::content::action content;
            constexpr static const auto id = content::id;
            
            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void*, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<content::types::request>();
                auto result = message::status::error::unknown;
                
                if (request != nullptr) {
                    auto info = context->userCtrl.get_info(request->accountID);
                    auto region = context->regionCtrl.get_region(0);
                    ::types::game_events action = (::types::game_events) request->actionID;
                    region::dynamic_obj* obj = region->action_for_dynamic_object(0, action);
                    if (obj != nullptr) {
                        connection::sender::send<::types::data_transfer::content::dynamic_object>(info->connection, *obj);
                    }
                    
                    return message::status::close;
                }

                return result;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                return message::status::error::unknown;
            }

        };
    }
}
