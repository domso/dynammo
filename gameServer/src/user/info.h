#ifndef gameServer_user_info_h
#define gameServer_user_info_h

#include <mutex>
#include "network/tcp_connection.h"

namespace user {
    class info {
    public:
        info();
        info(const info& copy) = delete;
        info(info&& move) = delete;
        ~info();
        
        std::mutex& mutex();
        
        network::tcp_connection<network::ipv4_addr> connection;
    private:
        std::mutex m_mutex;
    };
}

#endif
