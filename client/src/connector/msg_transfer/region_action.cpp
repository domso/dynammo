#include "src/connector/msg_transfer/region_action.h"
#include "src/connector/sign_buffer.h"


bool connector::msg_transfer::region_action::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    uint64_t eventArg,
    connector::context* context
) {
    auto request = outputBuffer.push_next<content::types::request>();

    if (request != nullptr) {
        request->sessionID = context->config.global().get<uint32_t>("sessionID").second;
        request->actionID = (::types::game_events)eventArg;
        request->regionID = 0;
        return sign_buffer(outputBuffer, *context);
    }

    return false;
}

message::msg_status_t connector::msg_transfer::region_action::requestHandler(
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
        auto dynObj = inputBuffer.get_next<region::dynamic_obj>();
        while (dynObj != nullptr) {
            context->gameCtrl.add_game_object(0, *dynObj);
            dynObj = inputBuffer.get_next<region::dynamic_obj>();
        }
    }
    

    return message::status::close;
}

message::msg_status_t connector::msg_transfer::region_action::responseHandler(
    message::msg_header_t& header, 
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    auto response = inputBuffer.get_next<content::types::response>();
    
    
    if (response != nullptr) {
        auto dynObj = inputBuffer.get_next<region::dynamic_obj>();
        while (dynObj != nullptr) {
            context->gameCtrl.add_game_object(0, *dynObj);
            dynObj = inputBuffer.get_next<region::dynamic_obj>();
        }
    }
    

    return message::status::close;
}



