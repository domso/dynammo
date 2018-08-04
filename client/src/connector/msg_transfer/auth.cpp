#include "src/connector/msg_transfer/auth.h"

#include "src/connector/sign_buffer.h"

bool connector::msg_transfer::auth::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    uint64_t eventArg,
    connector::context* context
) {
    auto request = outputBuffer.push_next<content::types::request>();

    if (request != nullptr) {
        request->accountID = context->config.global().get<uint32_t>("accountID").second;
        request->sessionID = context->config.global().get<uint32_t>("sessionID").second;
        context->privateKey.load(context->config.global().get<std::string>("privateKey").second);

        return sign_buffer(outputBuffer, *context);
    }

    return false;
}

message::msg_status_t connector::msg_transfer::auth::requestHandler(
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

message::msg_status_t connector::msg_transfer::auth::responseHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    context->config.global().set("Auth-State", header.status);
    if (header.status == message::status::ok) {
        context->eventCtrl.new_event(types::game_events::success_login);
    } else {
        context->eventCtrl.new_event(types::game_events::failure_login);
    }
    
    return message::status::close;
}
