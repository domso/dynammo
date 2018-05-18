#include "src/connector/sign_buffer.h"

bool connector::msg_transfer::sign_buffer(network::pkt_buffer& outputBuffer, session::controller& sessionCtrl) {
    int sigLength = sessionCtrl.get_signature_length();
    int msgLength = outputBuffer.msg_length();
    
    encryption::signature sig(outputBuffer.push_next<uint8_t>(sigLength), sigLength);
    return sessionCtrl.sign_data(sig, outputBuffer.data(), msgLength);
}
