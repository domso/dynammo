#include "src/region/context.h"
#include "src/util/file_storage.h"

region::context::context(const uint64_t id) : m_id(id) {
    load();
}

region::context::~context() {
    save();
}

void region::context::add_user(const authentication::accountID_t accountID) {
    m_activeUsers.insert(accountID);

}

void region::context::remove_user(const authentication::accountID_t accountID) {
    m_activeUsers.erase(accountID);
}

bool region::context::update() {
//     std::cout << "update region" << std::endl;
    
    return true;
    //m_regionData.update();
}

void region::context::load() {
    util::file_storage file;
    file.init("data/testLayer.layer");

    for (int i = 0; i < m_testLayer.size * m_testLayer.size; i++) {
        file.read<uint32_t>(&m_testLayer[i]);
    }
    
    region::dynamic_obj obj;
    obj.id = 0;
    obj.position.x = 5;
    obj.position.y = 5;
    obj.position.z = 0;
    
    insert_new_dynamic_object(obj);    
}

void region::context::save() {
}


region::dynamic_obj* region::context::action_for_dynamic_object(const uint32_t id, const types::game_events action) {   
    if (id < m_dynamicObjects2.size()) {
        if (m_dynamicObjects2[id].action(action)) {
            return &m_dynamicObjects2[id];
        }
    }
    
    return nullptr;
}

void region::context::insert_new_dynamic_object(const region::dynamic_obj& obj) {
    m_dynamicObjects2.push_back(obj);
}

region::layer<uint32_t>& region::context::get_layer() {
    return m_testLayer;
}

std::vector<region::dynamic_obj>& region::context::get_dynamic_obj() {
    std::cout << m_dynamicObjects2.size() << std::endl;
    return m_dynamicObjects2;
}
