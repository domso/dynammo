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
    
    region::dynamic_obj obj;
    obj.position.x = 5;
    obj.position.y = 5;
    obj.position.z = 0;
    
    insert_new_dynamic_object(obj);    
}

void region::data::save() {
}

void region::data::send_init(network::tcp_connection<network::ipv4_addr>& connection) {
    send_init_layer(connection);
    send_init_dynamic_objects(connection);
    std::cout << "send data" << std::endl;
}

void region::data::update(network::pkt_buffer& sendBuffer) {
}

void region::data::send_init_layer(network::tcp_connection<network::ipv4_addr>& connection) {
    uint8_t protocolID = 1;
    uint16_t count = 1;
    connection.send_data<uint8_t>(&protocolID, 1);
    connection.send_data<uint16_t>(&count, 1);
    connection.send_data<region::layer<uint32_t>>(&m_testLayer, 1);
}

void region::data::send_init_dynamic_objects(network::tcp_connection<network::ipv4_addr>& connection) {    
    uint8_t protocolID = 2;
    uint16_t count = m_dynamicObjects.size();
    connection.send_data<uint8_t>(&protocolID, 1);
    connection.send_data<uint16_t>(&count, 1);
    for (auto& kv : m_dynamicObjects) {
        connection.send_data<dynamic_obj>(&kv.second, 1);
    }
}

void region::data::insert_new_dynamic_object(const region::dynamic_obj& obj) {
    m_dynamicObjects.insert_or_assign(obj.id, obj);
}

