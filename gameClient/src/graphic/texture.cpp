#include "texture.h"

graphic::texture::texture(const std::string& filename) {    
    m_image.set(filename);
    m_valid = false;
}

graphic::texture::texture(graphic::texture && move) : m_image(std::move(move.m_image)), m_valid(move.m_valid), m_textureHandle(move.m_textureHandle) {
    move.m_valid = false;
}


graphic::texture::~texture() {
    if (m_valid) {
        glDeleteTextures(1, &m_textureHandle);
    }
}


void graphic::texture::link() {
    m_valid = true;
    glGenTextures(1, &m_textureHandle);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto pixelData = m_image.get_pixbuf();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelData->get_width(), pixelData->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData->get_pixels());
}

void graphic::texture::bind() {
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}



