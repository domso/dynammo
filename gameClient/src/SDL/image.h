#ifndef gameClient_SDL_image_h
#define gameClient_SDL_image_h

#include <string>
#include <SDL2/SDL.h>
#include "renderer.h"

namespace SDL {
    class image {
    public:
        image();
        image(const image& o) = delete;
        ~image();
        
        bool load(const std::string& filename, renderer& context);     
        
        SDL_Texture* internal_handle();
    private:
        SDL_Surface* m_internalSurface;
        SDL_Texture* m_internalTexture;
    };   
}

#endif
