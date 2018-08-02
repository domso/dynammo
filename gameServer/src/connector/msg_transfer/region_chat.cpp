#include "src/connector/msg_transfer/region_chat.h"

bool connector::msg_transfer::region_chat::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    void*,
    connector::context* context
) {
    return false;
}

message::msg_status_t connector::msg_transfer::region_chat::requestHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    auto request = inputBuffer.get_next<content::types::request>();
    
    if (request != nullptr) {
        auto user = context->sessionCtrl.get_user(request->sessionID);
        auto msg = inputBuffer.get_next<char>(request->length);
        
        if (user && msg != nullptr) {
            if ((*user)->verify_buffer(inputBuffer)) {
                auto region = context->regionCtrl.get_region(request->regionID);               
                (*user).release();
                
                for (auto& userID : region->all_users()) {
                    auto dest = context->sessionCtrl.get_user(userID);
                    if (dest) {
                        (*dest)->send<::types::data_transfer::content::region_chat>(msg, request->length);
                    }
                }               

                return message::status::close;
            } else {
                return message::status::error::access_denied;
            }
        } else {
            return message::status::error::unavailable;
        }
    }
    
    return message::status::error::unknown;
}

message::msg_status_t connector::msg_transfer::region_chat::responseHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    return message::status::close;
}

