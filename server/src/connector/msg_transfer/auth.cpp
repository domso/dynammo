#include "src/connector/msg_transfer/auth.h"


bool connector::msg_transfer::auth::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    void*,
    connector::context* context
) {
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
    auto request = inputBuffer.get_next<content::types::request>();
    auto result = message::status::error::unknown;

    if (request != nullptr) {
        auto user = context->sessionCtrl.get_user(request->sessionID);

        if (user) {
            if ((*user)->set_accountID(request->accountID)) {
                if ((*user)->verify_buffer(inputBuffer)) {
                    (*user)->set_addr(srcAddr);
                    result = message::status::ok;
                } else {
                    result = message::status::error::access_denied;
                }
            } else {
                result = message::status::error::unavailable;
            }
        } else {
            result = message::status::error::unavailable;
        }
    }

    header.status = result;
    return result;
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
    return message::status::close;
}


