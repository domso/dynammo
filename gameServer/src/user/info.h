#ifndef gameServer_user_info_h
#define gameServer_user_info_h

#include <mutex>
#include "src/util/lock_ref.h"
#include "network/tcp_connection.h"

namespace user {
    class info : public util::locked_ref_item {
    public:
        info();
        info(const info& copy) = delete;
        info(info&& move) = delete;
        ~info();
                
        network::tcp_connection<network::ipv4_addr> connection;
    };
}

#endif
