#pragma once

#include "network/pkt_buffer.h"
#include "src/session/controller.h"

namespace connector {
    namespace msg_transfer {
        bool sign_buffer(network::pkt_buffer& outputBuffer, session::controller& sessionCtrl);
    }
}
