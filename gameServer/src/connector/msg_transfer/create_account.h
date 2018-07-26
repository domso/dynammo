#pragma once

#include <random>
#include <fstream>

#include "network/udp_socket.h"
#include "src/types/data_transfer/content.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/connection/sender.h"
#include "src/connector/verify_buffer.h"
#include "src/util/file_storage.h"

namespace connector {
    namespace msg_transfer {
        class create_account {
        public:
            constexpr static const uint8_t id = types::msg_transfer::content::ids::create_account;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void*, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {                
                auto response = outputBuffer.push_next<authentication::accountID_t>();
                message::msg_status_t result = message::status::error::unknown;
                                
                if (response != nullptr) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<authentication::accountID_t> dis(1);         
                    
                    do {
                        *response = dis(gen);
                    } while(std::ifstream("keys/" + std::to_string(*response) + ".pem"));
                                        
                    std::ofstream file("keys/" + std::to_string(*response) + ".pem");
                    
                    if (file.is_open()) {
                        if (file.write((char*)inputBuffer.data_offset(), inputBuffer.remaining_msg_length())) {
                            result = message::status::ok;
                        }                        
                    }               
                }
                
                header.status = result;
                return header.status;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                return message::status::error::unknown;
            }
        };
    }
}


