#include "src/region/context.h"

#include "src/util/mem.h"

region::context::context(const uint32_t regionID, graphic::controller& graphicCtrl, session::controller& sessionCtrl) : m_regionID(regionID), m_graphicCtrl(graphicCtrl), m_sessionCtrl(sessionCtrl), m_layers(1) {

}

region::context::~context() {

}

void region::context::load_layers(std::vector<region::layer<uint32_t>>&&  move) {
    m_layers = move;
    m_graphicCtrl.add_obj<region::layer<uint32_t>>(m_regionID, &m_layers[0], m_regionID);
}

void region::context::load_static_objects(std::vector<region::static_obj>&& objs) {
    for (auto& obj : objs) {
        if (m_staticObjects.insert_or_assign(obj.id(), obj).second) {
            m_graphicCtrl.add_obj<region::static_obj, region::layer<uint32_t>>(m_regionID, &obj, obj.id(), &m_layers[0]);
        } else {
            m_graphicCtrl.update_obj<region::static_obj>(m_regionID, &obj, obj.id());
        }
    } 
}

void region::context::load_dynamic_objects(std::vector<region::dynamic_obj>&& objs) {            
    for (auto& obj : objs) {
        load_dynamic_object(obj);
    }
}

void region::context::load_dynamic_object(const region::dynamic_obj& obj) {            
    select_character(obj.ownerID, obj.id);
    if (m_dynamicObjects.insert_or_assign(obj.id, obj).second) {
        m_graphicCtrl.add_obj<region::dynamic_obj, region::layer<uint32_t>>(m_regionID, &obj, obj.id, &m_layers[0]);
    } else {
        m_graphicCtrl.update_obj<region::dynamic_obj>(m_regionID, &obj, obj.id);
    }
}

void region::context::remove_dynamic_object(const uint32_t id) {
    if (m_dynamicObjects.count(id) != 0) {        
        m_graphicCtrl.remove_obj<region::dynamic_obj>(m_regionID, id);
        m_dynamicObjects.erase(id);
    }
}

bool region::context::update() {
    return true;
}

region::layer<uint32_t>& region::context::operator[](const int i) {
    return m_layers[i];
}

const region::layer<uint32_t>& region::context::operator[](const int i) const {
    return m_layers[i];
}

int region::context::size() const {
    return m_layers.size();
}

void region::context::select_character(const authentication::accountID_t ownerID, const uint32_t objID) {
    if (m_sessionCtrl.get_accountID() == ownerID) {
        m_selectedCharacter = objID;
    }
}

region::dynamic_obj& region::context::get_dynamic_obj(const uint32_t id) {
    return m_dynamicObjects[id];
}


uint32_t region::context::get_selected_character() const {
    return m_selectedCharacter;
}
