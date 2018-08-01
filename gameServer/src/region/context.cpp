#include "src/region/context.h"
#include "src/util/file_storage.h"

#include <random>

region::context::context(const uint32_t id) : m_id(id) {
    load();
}

region::context::~context() {
    save();
}

bool region::context::update() {
    return !m_activeUsers.empty();
}

bool region::context::action(const uint32_t accountID, const uint32_t sessionID, const types::game_events event) {
    switch(event) {
        case types::game_events::enter_region: {
            m_activeUsers.insert(sessionID);
            m_affectedUsers.insert(sessionID);
            
            m_dynamicObjects[sessionID].position.x = 5;
            m_dynamicObjects[sessionID].position.y = 5;
            m_dynamicObjects[sessionID].id = sessionID;            
            
            for (auto& item : m_staticObjects) {
                m_changedStaticObjects.insert(item.first);
            }
            
            for (auto& item : m_dynamicObjects) {
                m_changedDynamicObjects.insert(item.first);
            }
            
            m_changedLayers.insert(0);
            m_changedLayers.insert(1);
            
            break;
        }
        case types::game_events::leave_region: {
            m_activeUsers.erase(sessionID);
            m_affectedUsers.insert(sessionID);
            break;
        }
        case types::game_events::move_up: {
            m_dynamicObjects[sessionID].position.x -= 1;
            m_dynamicObjects[sessionID].animation = types::game_animations::move_up;
            m_affectedUsers.insert(sessionID);
            m_changedDynamicObjects.insert(sessionID);
            break;
        }
        case types::game_events::move_down: {
            m_dynamicObjects[sessionID].position.x += 1;
            m_dynamicObjects[sessionID].animation = types::game_animations::move_down;
            m_affectedUsers.insert(sessionID);
            m_changedDynamicObjects.insert(sessionID);
            break;
        }
        case types::game_events::move_left: {
            m_dynamicObjects[sessionID].position.y -= 1;
            m_dynamicObjects[sessionID].animation = types::game_animations::move_left;
            m_affectedUsers.insert(sessionID);
            m_changedDynamicObjects.insert(sessionID);
            break;
        }
        case types::game_events::move_right: {
            m_dynamicObjects[sessionID].position.y += 1;
            m_dynamicObjects[sessionID].animation = types::game_animations::move_right;
            m_affectedUsers.insert(sessionID);
            m_changedDynamicObjects.insert(sessionID);
            break;
        }
        
        default: {
            
        }
    }   
    
    return true;
}

void region::context::remove_user(const uint32_t sessionID) {
    m_activeUsers.erase(sessionID);
}

const std::unordered_set<uint32_t>& region::context::all_users() {
    return m_activeUsers;
}

const std::unordered_set<uint32_t>& region::context::affected_users() {
    return m_affectedUsers;
}

const std::unordered_set<uint32_t>& region::context::changed_dynamic_objects() {
    return m_changedDynamicObjects;
}

const std::unordered_map<uint32_t, region::dynamic_obj>& region::context::all_dynamic_objects() {
    return m_dynamicObjects;
}

const std::unordered_set<uint32_t>& region::context::changed_static_objects() {
    return m_changedStaticObjects;
}

const std::unordered_map<uint32_t, region::static_obj>& region::context::all_static_objects() {
    return m_staticObjects;
}

const std::set<uint32_t>& region::context::changed_layers() {
    return m_changedLayers;
}

const std::vector<region::layer<uint32_t>>& region::context::all_layers() {
    return m_layers;
}

void region::context::commit() {
    m_affectedUsers.clear();
    m_changedDynamicObjects.clear();
    m_changedStaticObjects.clear();    
}


void region::context::load_layer(region::layer<uint32_t>& layer, const std::string& filename) {
    util::file_storage file;

    file.init(filename);

    int whiteSpaceCounter = 0;

    while (whiteSpaceCounter != 4) {
        char c;
        file.read<char>(&c);

        whiteSpaceCounter += c == ' ';
    }

    for (int i = 0; i < layer.size; i++) {
        uint8_t c;
        file.read<uint8_t>(&c);

        layer[i] = c;
    }
}

void region::context::load() {
    m_layers.resize(2);
    load_layer(m_layers[0], "data/terrain.ppm");
    load_layer(m_layers[1], "data/water.ppm");
    
//     for (int i = 0; i < 10000; i++) {
//         m_staticObjects[i].durability = 100;
//         m_staticObjects[i].type = 1;
//         m_staticObjects[i].position.x = i % 512;
//         m_staticObjects[i].position.y = i / 512;
//     }
}

void region::context::save() {

}
