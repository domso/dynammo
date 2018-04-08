#pragma once

#include "network/udp_socket.h"
#include "src/types/data_transfer/content.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/connection/sender.h"
#include "src/connector/msg_transfer/verify_buffer.h"

namespace connector {
    namespace msg_transfer {
        class auth {
        public:
            typedef ::types::msg_transfer::content::auth content;
            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void*, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<content::types::request>();
                auto response = outputBuffer.push_next<content::types::response>();
                auto result = message::status::error::unknown;

                if (request != nullptr && response != nullptr) {
                    response->accountID = request->accountID;
                    {
                        auto info = context->userCtrl.get_info(request->accountID);
                        if (verify_buffer(inputBuffer, *info.data())) {
                            info->connection = context->connectionCtrl.get_new_connection(request->tcpTicket);

                            auto region = context->regionCtrl.get_region(0);
                            region->add_user(request->accountID);

                            connection::sender::send<::types::data_transfer::content::region_layer>(info->connection, region->get_layer());
                            connection::sender::send<::types::data_transfer::content::static_object>(info->connection, region->get_static_obj());
                            connection::sender::send<::types::data_transfer::content::dynamic_object>(info->connection, region->get_dynamic_obj());                            
                            
                            result = message::status::ok;
                        } else {
                            result = message::status::error::auth;
                        }
                    }
                }

                header.status = result; 
                return result;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                return message::status::error::unknown;
            }

        };
    }
}
