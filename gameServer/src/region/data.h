#ifndef gameServer_region_data_h
#define gameServer_region_data_h

#include <unordered_map>
#include "network/pkt_buffer.h"
#include "network/tcp_connection.h"
#include "src/region/layer.h"
#include "src/region/dynamic_obj.h"

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
        void send_init_layer(network::tcp_connection<network::ipv4_addr>& connection);
        void send_init_dynamic_objects(network::tcp_connection<network::ipv4_addr>& connection);
        
        void insert_new_dynamic_object(const region::dynamic_obj& obj);
        
        
        region::layer<uint32_t> m_testLayer;
        std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
    };
}

#endif
