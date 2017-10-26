#include "image.h"

#include "SDL2/SDL_image.h"

namespace SDL {
    image::image() : m_internalSurface(nullptr), m_internalTexture(nullptr){
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    }

    image::~image() {
        if (m_internalTexture != nullptr) {
            SDL_DestroyTexture(m_internalTexture);
        }
        
        if (m_internalSurface != nullptr) {
            SDL_FreeSurface(m_internalSurface);
        }
        
        IMG_Quit();
    }
    
    bool image::load(const std::string& filename, renderer& context) {
        bool result = false;
        
        m_internalSurface = IMG_Load(filename.c_str());        
        if (m_internalSurface != nullptr) {
            m_internalTexture = SDL_CreateTextureFromSurface(context.internal_handle(), m_internalSurface);
            result = m_internalTexture != nullptr;
        }
        
        return result;
    }    
    
    SDL_Texture* image::internal_handle() {
        return m_internalTexture;
    }
}
