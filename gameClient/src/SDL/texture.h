#ifndef gameClient_SDL_texture_h
#define gameClient_SDL_texture_h

#include <string>
#include <SDL2/SDL.h>
#include "image.h"

namespace SDL {
    class texture {
    public:
        texture(SDL_Texture* tex);
        texture(texture&& o);
        texture(const texture& o) = delete;
        ~texture();   
        
        SDL_Texture* internal_handle();
    private:
        SDL_Texture* m_internalTexture;
    };   
}

#endif
