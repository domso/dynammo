#include "src/connector/verify_buffer.h"

bool connector::msg_transfer::verify_buffer(network::pkt_buffer& inputBuffer, user::info& userData) {
    bool result = false;
    
    if (userData.publicKey.valid()) {
        int sigLength = userData.publicKey.required_size();        
        result = userData.publicKey.verify<int8_t>(inputBuffer.get_next<uint8_t>(sigLength), sigLength, inputBuffer.data(), inputBuffer.msg_length() - sigLength);
    }
    
    return result;
}



