#pragma once

#include "network/pkt_buffer.h"
#include "src/user/info.h"

namespace connector {
    namespace msg_transfer {
        bool verify_buffer(network::pkt_buffer& inputBuffer, user::info& userData);
    }
}
