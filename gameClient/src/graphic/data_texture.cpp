#include "src/graphic/data_texture.h"
#include <iostream>

graphic::data_texture::data_texture(const uint32_t* data, const int width, const int height) : m_data(data), m_width(width), m_height(height) {

}

graphic::data_texture::data_texture(graphic::data_texture&&  move) : m_data(move.m_data), m_width(move.m_width), m_height(move.m_height) {
    move.m_data = nullptr;
    move.m_width = 0;
    move.m_height = 0;
}

void graphic::data_texture::copy() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
}

