#pragma once

#include "src/graphic/base_texture.h"

namespace graphic {
    class img_texture : public base_texture {
    public:
        img_texture();
        img_texture(const std::string& filename);
        img_texture(const img_texture& copy) = delete;
        img_texture(img_texture&& move);

        void set(const std::string& filename);
        
        int width() const;
        int height() const;
        
        void copy();
    private:
        Gtk::Image m_image;
    };
}
