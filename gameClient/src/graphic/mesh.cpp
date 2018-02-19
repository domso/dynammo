#include "mesh.h"
#include <iostream>


#include <glm/glm.hpp>

graphic::mesh::mesh() : m_shaders(nullptr) {
}

graphic::mesh::mesh(graphic::mesh&& move) {
    //TODO add move!
}

graphic::mesh::~mesh() {
    unrealize();
}

void graphic::mesh::set_vertex_attr(const int attrNr, const float value) {
    if (m_internalVertexBuffer.size() <= attrNr) {
        m_internalVertexBuffer.resize(attrNr + 1);
    }

    m_internalVertexBuffer[attrNr].push_back(value);
}


void graphic::mesh::set_vertex_attr_dimension(const int attrNr, const int dim) {
    if (m_externalVertexAttrDimension.size() <= attrNr) {
        m_externalVertexAttrDimension.resize(attrNr + 1);
    }

    m_externalVertexAttrDimension[attrNr] = dim;
}

void graphic::mesh::set_shader_program(graphic::shader_program* prog) {
    m_shaders = prog;
}

void graphic::mesh::add_texture(graphic::texture* tex) {
    if (tex != nullptr) {
        m_textures.push_back(tex);
    }
}

void graphic::mesh::realize() {
    unrealize();

    m_shaders->add_shader<graphic::shader_program::shader_types::vertex>("shader/vertex.glsl");
    m_shaders->add_shader<graphic::shader_program::shader_types::fragment>("shader/fragment.glsl");
    m_shaders->add_uniform_attr("tex");

    
    if (m_shaders != nullptr) {
        m_shaders->link();
    }

    for (texture* tex : m_textures) {
        tex->link();
    }

    m_externalVertexBuffer.resize(m_internalVertexBuffer.size());
    m_externalVertexGroup.resize(m_internalVertexBuffer.size());
    m_externalVertexAttrDimension.resize(m_internalVertexBuffer.size());

    // group == Array
    glGenVertexArrays(m_externalVertexGroup.size(), m_externalVertexGroup.data());

    for (int i = 0; i < m_externalVertexGroup.size(); i++) {
        glBindVertexArray(m_externalVertexGroup[i]);
        glGenBuffers(1, &(m_externalVertexBuffer[i]));

        glBindBuffer(GL_ARRAY_BUFFER, m_externalVertexBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, m_internalVertexBuffer[i].size() * sizeof(m_internalVertexBuffer[i][0]), m_internalVertexBuffer[i].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
void graphic::mesh::unrealize() {
    if (m_externalVertexGroup.size() > 0) {
        glDeleteBuffers(m_externalVertexGroup.size(), m_externalVertexGroup.data());
    }

    m_externalVertexBuffer.clear();
    m_externalVertexGroup.clear();
}
float t;
void graphic::mesh::render() {

    if (m_shaders != nullptr) {
        m_shaders->enable();
        glActiveTexture(GL_TEXTURE0);

        for (texture* tex : m_textures) {
            tex->bind();
        }

        m_shaders->set_uniform_attr<int>("tex", 0);

        m_shaders->disable();
    }

    for (int i = 0; i < m_externalVertexGroup.size(); i++) {
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, m_externalVertexGroup[i]);
        glVertexAttribPointer(i, m_externalVertexAttrDimension[i], GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    if (m_shaders != nullptr) {
        m_shaders->enable();
    }

    if (m_internalVertexBuffer.size() != 0) {
        glDrawArrays(GL_TRIANGLES, 0, m_internalVertexBuffer[0].size() / 3);
    }

    if (m_shaders != nullptr) {
        m_shaders->disable();
    }

    for (int i = 0; i < m_externalVertexGroup.size(); i++) {
        glDisableVertexAttribArray(i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);   
    
}
/*


void render() {

    float mvp[16];

    glUseProgram(m_Program);

    glUniformMatrix4fv(m_Mvp, 1, GL_FALSE, &mvp[0]);


    float x = 1;
//         glUniform4fv(m_data, 1, &x);


//         glUniform1f(m_data, 1);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);




    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_data);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);



    glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size() * 3);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


    glUseProgram(0);
}

*/
