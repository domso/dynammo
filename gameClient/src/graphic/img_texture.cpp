#include "src/graphic/img_texture.h"


graphic::img_texture::img_texture(const std::string& filename) {
    m_image.set(filename);
}

graphic::img_texture::img_texture(graphic::img_texture&&  move) : m_image(std::move(move.m_image)) {

}

int graphic::img_texture::width() const {
    return m_image.get_pixbuf()->get_width();
}

int graphic::img_texture::height() const {
    return m_image.get_pixbuf()->get_height();
}


void graphic::img_texture::copy() {
    auto pixelData = m_image.get_pixbuf();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelData->get_width(), pixelData->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData->get_pixels());
}
