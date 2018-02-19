#include "src/region/context.h"
#include <iostream>

#include "src/util/mem.h"

region::context::context() : m_layers(1) {
    
}

region::context::~context() {
    
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
