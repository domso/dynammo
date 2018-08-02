#include "src/connector/msg_transfer/region_chat.h"
#include "src/connector/sign_buffer.h"


bool connector::msg_transfer::region_chat::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    uint64_t eventArg,
    connector::context* context
) {
    auto request = outputBuffer.push_next<content::types::request>();

    if (request != nullptr) {
        std::string msg = context->config.global().get<std::string>("chat-message").second;
        
        request->sessionID = context->config.global().get<uint32_t>("sessionID").second;
        request->regionID = 0;        
        request->length = msg.length();
        
        outputBuffer.push_string(msg);        
        
        return sign_buffer(outputBuffer, *context);
    }

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




