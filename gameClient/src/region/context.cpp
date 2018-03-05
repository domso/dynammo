#include "src/region/context.h"
#include <iostream>

#include "src/util/mem.h"

region::context::context(graphic::controller& graphicCtrl) : m_graphicCtrl(graphicCtrl), m_layers(1) {

}

region::context::~context() {

}

void region::context::load_layers(std::vector<region::layer<uint32_t> >&&  move) {
    m_layers = move;
    m_graphicCtrl.add_obj<region::layer<uint32_t>>(&m_layers[0], 0);
}

void region::context::load_dynamic_objects(std::vector<region::dynamic_obj>& objs) {
    int id = 1;

    for (auto& obj : objs) {
        m_dynamicObjects.insert_or_assign(obj.id, obj);
        std::cout << obj.position.x << " / " << obj.position.y << std::endl;
                
        std::pair<region::dynamic_obj*, region::layer<uint32_t>*> pair = std::make_pair<region::dynamic_obj*, region::layer<uint32_t>*>(&obj, &m_layers[0]);
        m_graphicCtrl.add_obj<std::pair<region::dynamic_obj*, region::layer<uint32_t>*>>(&pair, id++);
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
