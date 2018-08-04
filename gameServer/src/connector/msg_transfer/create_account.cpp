#include "src/connector/msg_transfer/create_account.h"

#include <random>
#include <fstream>
#include "src/util/file_storage.h"


bool connector::msg_transfer::create_account::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    void*,
    connector::context* context
) {
    return false;
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
    auto response = outputBuffer.push_next<content::types::response>();
    message::msg_status_t result = message::status::error::unknown;

    if (response != nullptr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(1);

        do {
            response->accountID = dis(gen);
        } while (std::ifstream("keys/" + std::to_string(response->accountID) + ".pem"));

        std::ofstream file("keys/" + std::to_string(response->accountID) + ".pem");

        if (file.is_open()) {
            if (file.write((char*)inputBuffer.data_offset(), inputBuffer.remaining_msg_length())) {
                result = message::status::ok;
            }
        }
    }

    header.status = result;
    return header.status;
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
    return message::status::error::unknown;
}


