#pragma once

#include "network/pkt_buffer.h"
#include "src/session/user.h"

namespace connector {
    namespace msg_transfer {
        bool verify_buffer(network::pkt_buffer& inputBuffer, session::user& userData);
    }
}