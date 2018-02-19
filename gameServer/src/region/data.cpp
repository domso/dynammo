#include "src/region/data.h"
#include "src/util/file_storage.h"

region::data::data() {
}

region::data::~data() {
}

void region::data::load(const uint64_t id) {
    util::file_storage file;
    file.init("data/testLayer.layer");
        
    for (int i = 0; i < m_testLayer.size * m_testLayer.size; i++) {
        file.read<uint32_t>(&m_testLayer[i]);
    }
}

void region::data::save() {
}

void region::data::send_init(network::tcp_connection<network::ipv4_addr>& connection) {
    uint8_t numLayers = 1;
    connection.send_data<uint8_t>(&numLayers, 1);
    connection.send_data<region::layer<uint32_t>>(&m_testLayer, 1);
    std::cout << "send data" << std::endl;
}

void region::data::update(network::pkt_buffer& sendBuffer) {
}
