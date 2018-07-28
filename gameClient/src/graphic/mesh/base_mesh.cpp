#include "src/graphic/mesh/base_mesh.h"
#include <glm/glm.hpp>

#include "src/util/int.h"

graphic::base_mesh::base_mesh() {
}

graphic::base_mesh::~base_mesh() {

}

void graphic::base_mesh::set_id(const uint32_t id) {
    m_id = id;
}

uint32_t graphic::base_mesh::get_id() const {
    return m_id;
}

void graphic::base_mesh::add_vertex_attr(const uint attrNr, const float value) {
    if (m_internalVertexBuffer.size() <= attrNr) {
        m_internalVertexBuffer.resize(attrNr + 1);
    }

    m_internalVertexBuffer[attrNr].push_back(value);
}

void graphic::base_mesh::set_vertex_attr(const uint attrNr, const uint vertexNr, const float value) {
    if (m_internalVertexBuffer.size() <= attrNr) {
        m_internalVertexBuffer.resize(attrNr + 1);
    }

    if (m_internalVertexBuffer[attrNr].size() <= vertexNr) {
        m_internalVertexBuffer[attrNr].resize(vertexNr + 1);
    }

    m_internalVertexBuffer[attrNr][vertexNr] = value;
}

void graphic::base_mesh::set_vertex_attr_dimension(const uint attrNr, const uint dim) {
    if (m_externalVertexAttrDimension.size() <= attrNr) {
        m_externalVertexAttrDimension.resize(attrNr + 1);
    }

    m_externalVertexAttrDimension[attrNr] = dim;
}

void graphic::base_mesh::add_texture(graphic::base_texture& tex) {
    m_textures.push_back(&tex);
}

void graphic::base_mesh::realize() {
    load();

    for (base_texture* tex : m_textures) {
        tex->link();
    }

    m_externalVertexBuffer.resize(m_internalVertexBuffer.size());
    m_externalVertexAttrDimension.resize(m_internalVertexBuffer.size());

    // group == Array
    glGenVertexArrays(1, &m_externalVertexGroup);
    glBindVertexArray(m_externalVertexGroup);

    for (uint i = 0; i < m_externalVertexBuffer.size(); i++) {
        glGenBuffers(1, &(m_externalVertexBuffer[i]));
        glBindBuffer(GL_ARRAY_BUFFER, m_externalVertexBuffer[i]);

        glBufferData(GL_ARRAY_BUFFER, m_internalVertexBuffer[i].size() * sizeof(m_internalVertexBuffer[i][0]), m_internalVertexBuffer[i].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
void graphic::base_mesh::unrealize() {
    free();
 
    m_shaders.close();
    
//     for (base_texture* tex : m_textures) {
//         tex->free();
//     }   
    
    glBindVertexArray(m_externalVertexGroup);
         
    for (uint i = 0; i < m_externalVertexBuffer.size(); i++) {
         glDeleteBuffers(1, &(m_externalVertexBuffer[i]));        
    }
    
    glDeleteVertexArrays(1, &m_externalVertexGroup);

     m_externalVertexBuffer.clear();
}

void graphic::base_mesh::render(const graphic::settings& settings) {     
    m_shaders.enable();
    update(settings);
    
    int i = 0;
    for (base_texture* tex : m_textures) {
        glActiveTexture(GL_TEXTURE0 + i);
        tex->bind();
        
        i++;
    }
    
//     m_shaders.disable();
    glBindVertexArray(m_externalVertexGroup);
    for (uint i = 0; i < m_externalVertexBuffer.size(); i++) {
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, m_externalVertexBuffer[i]);
        glVertexAttribPointer(i, m_externalVertexAttrDimension[i], GL_FLOAT, GL_FALSE, 0, nullptr);
    }



//     m_shaders.enable();


    if (m_internalVertexBuffer.size() != 0) {
        glDrawArrays(GL_TRIANGLES, 0, m_internalVertexBuffer[0].size() / 3);
    }

//     m_shaders.disable();
// 
// 
//     for (uint i = 0; i < m_externalVertexBuffer.size(); i++) {
//         glDisableVertexAttribArray(i);
//     }
// 
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
}
