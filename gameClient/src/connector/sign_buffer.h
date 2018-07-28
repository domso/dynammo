#pragma once

#include "network/pkt_buffer.h"

namespace connector {
    namespace msg_transfer {
        bool sign_buffer(network::pkt_buffer& outputBuffer);
    }
}
