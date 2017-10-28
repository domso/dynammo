#include "texture.h"

#include "SDL2/SDL_image.h"

namespace SDL {    
    texture::texture(SDL_Texture* tex) : m_internalTexture(tex) {
        
    }
    
    texture::texture(texture&& tex) {
        m_internalTexture = tex.m_internalTexture;
        tex.m_internalTexture = nullptr;
    }

    texture::~texture() {
        if (m_internalTexture != nullptr) {
            SDL_DestroyTexture(m_internalTexture);
        }
    }
    
    SDL_Texture* texture::internal_handle() {
        return m_internalTexture;
    }
}
