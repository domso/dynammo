#pragma once

#include <random>
#include <fstream>

#include "network/udp_socket.h"
#include "src/types/data_transfer/content.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/util/file_storage.h"

#include <iostream>

namespace connector {
    namespace msg_transfer {
        class create_account {
        public:
            constexpr static const uint8_t id = types::msg_transfer::content::ids::create_account;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, uint64_t eventArg, connector::msg_transfer::data* data) {
                util::file_storage file;
                
                if (file.init("../../keys/" + data->config.get<std::string>("username").second + "_public.pem")) {
                    char c;
                    while (file.read(&c)) {
                        char* buff = outputBuffer.push_next<char>();
                        if (buff != nullptr) {
                            *buff = c;
                        }
                    } 
                }                
                
                return true;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {                
                return message::status::close;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {
                uint32_t* accountID = inputBuffer.get_next<uint32_t>();
                
                if (header.status == message::status::ok && accountID != nullptr) {
                    data->config.set<uint32_t>("accountID", *accountID);
                    data->eventCtrl.new_event(types::game_events::success_account_creation);                    
                } else {
                    data->eventCtrl.new_event(types::game_events::failure_account_creation);
                }
                
                return message::status::close;
            }
        };
    }
}


