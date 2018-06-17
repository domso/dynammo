#include "src/graphic/img_block_texture.h"
#include <assert.h>
#include <string.h>

#include "src/util/int.h"

graphic::img_block_texture::img_block_texture(const std::string& filename, const int first, const int last, const int dimension) {    
    for (int i = first; i <= last; i++) {
        m_images.emplace_back(filename + std::to_string(i) + ".png");
    }
    
    assert(m_images.size() > 0);
    
    
    m_blockWidth = m_images[0].get_pixbuf()->get_width();
    m_blockHeight = m_images[0].get_pixbuf()->get_height();
    
    m_width = dimension * m_blockWidth;
    m_height = dimension * m_blockHeight;
    
    m_pixelData.resize(m_width * m_height, 0xFF00FFFF);    
    
    
    group_img_block(dimension);
}

graphic::img_block_texture::img_block_texture(graphic::img_block_texture&&  move) : m_images(std::move(move.m_images)) {

}

void graphic::img_block_texture::copy() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixelData.data());
}


void graphic::img_block_texture::group_img_block(const int dimension) {
    int currentWidth;
    int currentHeight;
    int currentPosition = 0;
    
    for (uint i = 0; i < m_images.size(); i++) {
        currentPosition = (i % dimension) * m_blockWidth + (i / dimension) * m_blockHeight * m_width;        
        
        currentWidth = m_images[i].get_pixbuf()->get_width();
        currentHeight = m_images[i].get_pixbuf()->get_height();
        
        for (int j = 0; j < currentHeight; j++) {
            uint32_t* src = ((uint32_t*) m_images[i].get_pixbuf()->get_pixels()) + j * currentWidth;
            uint32_t* dest = m_pixelData.data() + currentPosition + j * (m_width);
            
            if (dest + currentWidth < m_pixelData.data() + m_pixelData.size()) {
                memcpy(dest, src, currentWidth * sizeof(uint32_t));
            }           
        }
    }
    
}
