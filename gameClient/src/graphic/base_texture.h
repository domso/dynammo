#ifndef gameClient_graphic_base_texture_h
#define gameClient_graphic_base_texture_h

#include <string>
#include <epoxy/gl.h>
#include <gtkmm/image.h>
              
namespace graphic {
    class base_texture {
    public:
        base_texture();
        base_texture(const base_texture& copy) = delete;
        base_texture(base_texture&& move);        
        ~base_texture();
            
        virtual void copy() = 0;
        
        void free();
        void link();
        void bind();
    private:        
        bool m_linked;
        GLuint m_textureHandle;
    };
}

#endif
