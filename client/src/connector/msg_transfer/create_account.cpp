#include "src/connector/msg_transfer/create_account.h"

#include <random>
#include <fstream>
#include "src/util/file_storage.h"

bool connector::msg_transfer::create_account::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    uint64_t eventArg,
    connector::context* context
) {
    util::file_storage file;

    if (file.init("../keys/" + context->config.global().get<std::string>("username").second + "_public.pem")) {
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

message::msg_status_t connector::msg_transfer::create_account::requestHandler(
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

message::msg_status_t connector::msg_transfer::create_account::responseHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    uint32_t* accountID = inputBuffer.get_next<uint32_t>();

    if (header.status == message::status::ok && accountID != nullptr) {
        context->config.global().set<uint32_t>("accountID", *accountID);
        context->eventCtrl.new_event(types::game_events::success_account_creation);
    } else {
        context->eventCtrl.new_event(types::game_events::failure_account_creation);
    }

    return message::status::close;
}



