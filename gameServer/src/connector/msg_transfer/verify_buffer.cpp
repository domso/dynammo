#include "src/connector/msg_transfer/verify_buffer.h"

bool connector::msg_transfer::verify_buffer(network::pkt_buffer& inputBuffer, user::info& userData) {
    bool result = false;
    
    if (userData.publicKey.valid()) {
        int sigLength = userData.publicKey.getRequiredSize();
        encryption::signature sig(inputBuffer.get_next<uint8_t>(sigLength), sigLength);
        result = encryption::verify<int8_t>(userData.publicKey, sig, inputBuffer.data(), inputBuffer.msg_length() - sigLength);
    }
    
    return result;
}



