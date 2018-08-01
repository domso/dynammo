#include "base_texture.h"

graphic::base_texture::base_texture() {
    m_linkCounter = 0;
}

graphic::base_texture::base_texture(graphic::base_texture&& move) : m_linkCounter(move.m_linkCounter), m_textureHandle(move.m_textureHandle) {
    std::lock_guard<std::mutex> lg(move.m_mutex);
    move.m_linkCounter = 0;
}

graphic::base_texture::~base_texture() {
    free();    
}

void graphic::base_texture::free() {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    if (m_linkCounter == 1) {
        glDeleteTextures(1, &m_textureHandle);
    }
    m_linkCounter--;
}

void graphic::base_texture::link() {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    if (m_linkCounter == 0) {
        glGenTextures(1, &m_textureHandle);
        glBindTexture(GL_TEXTURE_2D, m_textureHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        copy();
    }
    
    m_linkCounter++;
}

void graphic::base_texture::bind() {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    if (m_linkCounter > 0) {
        glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    }
}



