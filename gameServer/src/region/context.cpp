#include "src/region/context.h"
#include "src/util/file_storage.h"

region::context::context(const uint32_t id) : m_id(id) {
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
        m_testLayer[i] = 0;
    }

    if (m_id == 0) {

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
}

void region::context::save() {

}

uint32_t region::context::insert_new_dynamic_object(const region::dynamic_obj& obj) {
//     m_dynamicObjects2.push_back(obj);
//     m_dynamicObjects2.back().id = m_dynamicObjects2.size() - 1;
    uint32_t id = m_dynIdCounter++;
    auto& destObj = m_dynamicObjects[id];
    destObj = obj;
    destObj.id = id;
    
    return id;
}

void region::context::remove_dynamic_object(const uint32_t id) {
    m_dynamicObjects.erase(id);
}

void region::context::insert_new_static_object(const region::static_obj& obj) {
    m_staticObjects.push_back(obj);
}

const region::layer<uint32_t>& region::context::get_layer() const {
    return m_testLayer;
}


region::dynamic_obj* region::context::get_dynamic_obj(const uint32_t id) {
    auto it = m_dynamicObjects.find(id);

    if (it != m_dynamicObjects.end()) {
        return &it->second;
    }

    return nullptr;
}


// const std::vector<region::dynamic_obj>& region::context::get_dynamic_objs() const {
//     return m_dynamicObjects2;
// }

const std::unordered_map<uint32_t, region::dynamic_obj>& region::context::get_dynamic_objs() const {
    return m_dynamicObjects;
}

const std::vector<region::static_obj>& region::context::get_static_objs() const {
    return m_staticObjects;
}

