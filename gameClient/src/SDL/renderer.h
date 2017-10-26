#ifndef gameClient_src_SDL_renderer_h
#define gameClient_src_SDL_renderer_h

#include <SDL2/SDL.h>
#include "window.h"

namespace SDL {
    class renderer {
    public:        
        renderer();
        
        renderer(const renderer&) = delete;
        
        bool init(window& targetWindow);
        
        ~renderer();
        
        void clear();
        
        void update();
        
        template <typename T>
        void draw(T& texture, const rect* sourceRect = nullptr, const rect* destRect = nullptr) {
            SDL_RenderCopy(m_internalHandle, texture.internal_handle(), sourceRect, destRect);
        }
        
        void draw_line(const int srcX, const int srcY, const int destX, const int destY);        
        
        void set_draw_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        
        SDL_Renderer* internal_handle();
    private:
        SDL_Renderer* m_internalHandle;
    };
}

#endif
