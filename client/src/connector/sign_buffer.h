#pragma once

#include "network/pkt_buffer.h"
#include "src/connector/context.h"

namespace connector {
    namespace msg_transfer {
        bool sign_buffer(network::pkt_buffer& outputBuffer, connector::context& context);
    }
}
