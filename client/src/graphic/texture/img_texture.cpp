#include "src/graphic/texture/img_texture.h"

graphic::img_texture::img_texture() {

}

graphic::img_texture::img_texture(const std::string& filename) {
    m_image.set(filename);
}

graphic::img_texture::img_texture(graphic::img_texture&&  move) : m_image(std::move(move.m_image)) {

}

void graphic::img_texture::set(const std::string& filename) {
    m_image.set(filename);
}

int graphic::img_texture::width() const {
    return m_image.get_pixbuf()->get_width();
}

int graphic::img_texture::height() const {
    return m_image.get_pixbuf()->get_height();
}


void graphic::img_texture::copy() {
    auto imgType = m_image.get_storage_type();
    
    if (imgType == Gtk::IMAGE_PIXBUF) {
        auto pixelData = m_image.get_pixbuf();

        if (pixelData->get_has_alpha()) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelData->get_width(), pixelData->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData->get_pixels());
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelData->get_width(), pixelData->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData->get_pixels());
        }
    }
}
