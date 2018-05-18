#ifndef gameServer_user_info_h
#define gameServer_user_info_h

#include <mutex>
#include "network/tcp_connection.h"
#include "src/util/lock_ref.h"
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"


namespace user {
    class info : public util::locked_ref_item {
    public:
        info();
        info(const info& copy) = delete;
        info(info&& move) = delete;
        ~info();

        network::tcp_connection<network::ipv4_addr> connection;
        network::ipv4_addr udpAddr;
        encryption::public_key publicKey;
    };
}

#endif
