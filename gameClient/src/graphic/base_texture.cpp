#include "base_texture.h"

graphic::base_texture::base_texture() {
    m_linked = false;
}

graphic::base_texture::base_texture(graphic::base_texture&&  move) : m_linked(move.m_linked), m_textureHandle(move.m_textureHandle) {
    move.m_linked = false;
}


graphic::base_texture::~base_texture() {
    if (m_linked) {
        glDeleteTextures(1, &m_textureHandle);
    }
}

void graphic::base_texture::free() {
    if (m_linked) {
        glDeleteTextures(1, &m_textureHandle);
        m_linked = false;
    }
}

void graphic::base_texture::link() {
    if (!m_linked) {
        glGenTextures(1, &m_textureHandle);
        glBindTexture(GL_TEXTURE_2D, m_textureHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        copy();
        m_linked = true;
    }
}

void graphic::base_texture::bind() {
    if (m_linked) {
        glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    }
}



