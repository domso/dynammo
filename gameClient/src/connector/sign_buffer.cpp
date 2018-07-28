#include "src/connector/sign_buffer.h"

bool connector::msg_transfer::sign_buffer(network::pkt_buffer& outputBuffer, connector::context& context) {
    int sigLength = context.privateKey.required_size();
    int msgLength = outputBuffer.msg_length();
        
    return context.privateKey.sign<int8_t>(
        outputBuffer.push_next<uint8_t>(
            sigLength
        ),
        sigLength,
        outputBuffer.data(),
        msgLength
    ) > 0;
}
