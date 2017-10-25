#ifndef gameClient_src_SDL_renderer_h
#define gameClient_src_SDL_renderer_h

#include <SDL2/SDL.h>
#include "window.h"

namespace SDL {
    class renderer {
    public:        
        renderer() : m_internalHandle(nullptr) {
            
        }
        
        renderer(const renderer&) = delete;
        
        bool init(window& targetWindow) {
            m_internalHandle = SDL_CreateRenderer(targetWindow.internal_handler(), -1, SDL_RENDERER_ACCELERATED);
            return m_internalHandle != nullptr;
        }        
        
        ~renderer() {
            if (m_internalHandle != nullptr) {
                SDL_DestroyRenderer(m_internalHandle);
            }
        }
        
        void clear() {
            SDL_RenderClear(m_internalHandle);
        }
        
        void update() {
            SDL_RenderPresent(m_internalHandle);
        }
        
        
        SDL_Renderer* internal_handler() {
            return m_internalHandle;
        }       
    private:
        SDL_Renderer* m_internalHandle;
    }
}

#endif
