#include "renderer.h"

namespace SDL {  
    renderer::renderer() : m_internalHandle(nullptr) {
        
    }
    
    bool renderer::init(window& targetWindow) {
        m_internalHandle = SDL_CreateRenderer(targetWindow.internal_handler(), -1, SDL_RENDERER_ACCELERATED);
        return m_internalHandle != nullptr;
    }        
    
    renderer::~renderer() {
        if (m_internalHandle != nullptr) {
            SDL_DestroyRenderer(m_internalHandle);
        }
    }
    
    void renderer::clear() {
        SDL_RenderClear(m_internalHandle);
    }
    
    void renderer::update() {
        SDL_RenderPresent(m_internalHandle);
    }      
        
    void renderer::draw(texture& tex, const rect* sourceRect, const rect* destRect) {
        SDL_RenderCopy(m_internalHandle, tex.internal_handle(), sourceRect, destRect);
    }
        
    void renderer::draw_line(const int srcX, const int srcY, const int destX, const int destY) {
        SDL_RenderDrawLine(m_internalHandle, srcX, srcY, destX, destY);
    }
    
    void renderer::set_draw_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        SDL_SetRenderDrawColor(m_internalHandle, r, g, b, a);
    }
        
    texture renderer::create_texture(image& texImg) {
        texture tmp(SDL_CreateTextureFromSurface(m_internalHandle, texImg.internal_handle()));
        return tmp;
    }
        
    SDL_Renderer* renderer::internal_handle() {
        return m_internalHandle;
    }      
}

