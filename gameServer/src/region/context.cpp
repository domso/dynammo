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

const std::unordered_set<authentication::accountID_t>& region::context::get_users() const {
    return m_activeUsers;
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
    obj.ownerID = 1;
    obj.position.x = 5;
    obj.position.y = 5;
    obj.position.z = 0;
    
    insert_new_dynamic_object(obj); 
    
    obj.ownerID = 2;
    obj.position.x = 50;
    obj.position.y = 50;
    obj.position.z = 0;
    
    insert_new_dynamic_object(obj); 
        
    region::static_obj sObj;
    
    sObj.durability = 100;
    sObj.type = 0;
    sObj.position.x = 38;
    sObj.position.y = 6;
    sObj.position.z = 0;    
    
    insert_new_static_object(sObj);     
    
    sObj.durability = 100;
    sObj.type = 0;
    sObj.position.x = 90;
    sObj.position.y = 64;
    sObj.position.z = 0;    
    
    insert_new_static_object(sObj);  
    
    sObj.durability = 100;
    sObj.type = 0;
    sObj.position.x = 25;
    sObj.position.y = 50;
    sObj.position.z = 0;    
    
    insert_new_static_object(sObj);  
}

void region::context::save() {
    
}

region::dynamic_obj* region::context::action_for_dynamic_object(const uint32_t id, const types::game_events action, const authentication::accountID_t accountID) {   
    if (id < m_dynamicObjects2.size()) {        
        if (m_dynamicObjects2[id].ownerID == accountID && m_dynamicObjects2[id].action(action)) {
            return &m_dynamicObjects2[id];
        }
    }
    
    return nullptr;
}

void region::context::insert_new_dynamic_object(const region::dynamic_obj& obj) {
    m_dynamicObjects2.push_back(obj);
    m_dynamicObjects2.back().id = m_dynamicObjects2.size() - 1;
}

void region::context::insert_new_static_object(const region::static_obj& obj) {
    m_staticObjects.push_back(obj);
}

region::layer<uint32_t>& region::context::get_layer() {
    return m_testLayer;
}

std::vector<region::dynamic_obj>& region::context::get_dynamic_obj() {
    return m_dynamicObjects2;
}

std::vector<region::static_obj>& region::context::get_static_obj() {
    return m_staticObjects;
}

