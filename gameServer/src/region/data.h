#ifndef gameServer_region_data_h
#define gameServer_region_data_h

#include "network/pkt_buffer.h"
#include "network/tcp_connection.h"
#include "src/region/layer.h"


namespace region {
    class data {
    public:
        data();
        data(const data& copy) = delete;
        ~data();
        
        void load(const uint64_t id);
        void save();
        void send_init(network::tcp_connection<network::ipv4_addr>& connection);
        void update(network::pkt_buffer& sendBuffer);
    private:
        region::layer<uint32_t> m_testLayer;
    };
}

#endif
