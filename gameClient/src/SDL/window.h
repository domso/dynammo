#ifndef gameClient_SDL_window_h
#define gameClient_SDL_window_h

#include <SDL2/SDL.h>
#include <string>

namespace SDL {
    class window {
    public:        
        window() : m_internalHandle(nullptr) {
            
        }
        
        window(const window&) = delete;
        
        struct config {
            int width;
            int height;
            std::string title = "window";
            int x = SDL_WINDOWPOS_UNDEFINED;
            int y = SDL_WINDOWPOS_UNDEFINED;
            uint32_t flags = SDL_WINDOW_SHOWN;
        };
        
        bool init(const config& c) {
            m_internalHandle = SDL_CreateWindow(c.title.c_str(), c.x, c.y, c.width, c.height, c.flags);
            return m_internalHandle != nullptr;
        }        
        
        ~window() {
            if (m_internalHandle != nullptr) {
                SDL_DestroyWindow(m_internalHandle);
            }
        }
        
        SDL_Window* internal_handler() {
            return m_internalHandle;
        }        
        
    private:
        SDL_Window* m_internalHandle;
    }
}

        
    }   
}

#endif
