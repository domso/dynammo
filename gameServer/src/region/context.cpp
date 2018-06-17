#include "src/region/context.h"
#include "src/util/file_storage.h"

#include <random>

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
    file.init("data/export.ppm");
    
    int whiteSpaceCounter = 0;
    while (whiteSpaceCounter != 4) {
        char c;        
        file.read<char>(&c);
        
        whiteSpaceCounter += c == ' ';
    }
    

    for (int i = 0; i < m_terrainLayer.size; i++) {
        uint8_t c;        
        file.read<uint8_t>(&c);
        
        m_terrainLayer[i] = c;
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

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dis(0, 100);
        
        for (int i = 0; i < 100; i++) {
            sObj.durability = 100;
            sObj.type = 0;
            sObj.position.x = dis(mt);
            sObj.position.y = dis(mt);
            sObj.position.z = 0;

            insert_new_static_object(sObj);
        }

    }
}

void region::context::save() {

}

uint32_t region::context::insert_new_dynamic_object(const region::dynamic_obj& obj) {
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
    if (obj.position.z >= m_staticObjLayers.size()) {
        m_staticObjLayers.resize(obj.position.z + 1);
    }
    
    m_staticObjLayers[obj.position.z].get_nearest(obj.position.x, obj.position.y) = obj;
}

const region::layer<uint32_t>& region::context::get_layer() const {
    return m_terrainLayer;
}

const region::layer<region::static_obj>& region::context::get_obj_layer(const int level) const {
    return m_staticObjLayers[level];
}

region::dynamic_obj* region::context::get_dynamic_obj(const uint32_t id) {
    auto it = m_dynamicObjects.find(id);

    if (it != m_dynamicObjects.end()) {
        return &it->second;
    }

    return nullptr;
}

const std::unordered_map<uint32_t, region::dynamic_obj>& region::context::get_dynamic_objs() const {
    return m_dynamicObjects;
}

std::vector<region::static_obj> region::context::get_static_objs() const {
    std::vector<region::static_obj> result;
    
    for (auto& layer : m_staticObjLayers) {
        for (int i = 0; i < layer.size; i++) {
            if (layer[i].durability > 0) {
                result.push_back(layer[i]);
            }
        }
    }
    
    return result;
}

