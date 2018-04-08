#pragma once

#include <vector>
#include "src/graphic/base_texture.h"

namespace graphic {
    class img_block_texture : public base_texture {
    public:
        img_block_texture(const std::string& filename, const int first, const int last, const int dimension);
        img_block_texture(const img_block_texture& copy) = delete;
        img_block_texture(img_block_texture&& move);

        void copy();
    private:
        void group_img_block(const int dimension);
        
        std::vector<Gtk::Image> m_images;
        
        int m_width;
        int m_height;
        
        int m_blockWidth;
        int m_blockHeight;
        std::vector<uint32_t> m_pixelData;
    };
}

